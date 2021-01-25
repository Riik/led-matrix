#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <gpiod.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <linux/spi/spidev.h>

#include "pabort.h"

static const char *spidev = "/dev/spidev0.0";

/**On the pin layout:
 * SPI:
 *  SPI0 is in use, so that GPIO 10 is MOSI and GPIO11 is SCLK.
 * GPIO:
 *  14: S0
 *  15: S1
 *  18: S2
 *  12: r_clk
 */

static unsigned int sLineOffsets[] = {14, 15, 18};
static const int sLineDefaults[] = {0, 0, 0};
static const uint8_t data[8] = {1, 2, 4, 8, 16, 32, 64, 128};
static const uint8_t zeros = 0x0;


int main(int argc, char** argv)
{
    (void)argc;

    int spifd = open(spidev, O_RDWR);
    if (spifd < 0) {
        pabort("Opening spidev failed");
    }
    struct gpiod_chip* chip = gpiod_chip_open_by_name("gpiochip0");
    if (chip == NULL) {
        pabort("Opening gpiochip0 failed");
    }
    //Get S0, S1 and S2 in bulk
    struct gpiod_line_bulk sBulk;
    int suc = gpiod_chip_get_lines(chip, &sLineOffsets[0], sizeof(sLineOffsets)/sizeof(sLineOffsets[0]), &sBulk);
    if (suc != 0) {
        pabort("Getting the S-lines as bulk failed");
    }
    // Set the GPIO lines to output and 0
    suc = gpiod_line_request_bulk_output(&sBulk, argv[0], &sLineDefaults[0]);
    if (suc != 0) {
        pabort("Setting the gpio lines as output to 0 failed");
    }
    struct gpiod_line *rckLine = gpiod_chip_get_line(chip, 12);
    gpiod_line_request_output(rckLine, argv[0], 0);
    // Prepare the SPI output
    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));
    tr.len = 1;
    tr.delay_usecs = 100;
    tr.speed_hz = 5000000;
    tr.bits_per_word = 8;
    tr.tx_nbits = 1;
    tr.rx_nbits = 1;

    const struct timespec req = {
        .tv_sec = 0,
        .tv_nsec = 1000000, // 1 ms
    };
    while(true) {
        for (uint_fast8_t row = 0; row < 8; ++row) {
            int bulkValue[3] = {row & 0x1 ? 1 : 0, row & 0x2 ? 1 : 0, row & 0x4 ? 1 : 0};
            tr.tx_buf = (unsigned long)&zeros;
            suc = ioctl(spifd, SPI_IOC_MESSAGE(1), &tr);
            if (suc < 0) {
                pabort("Transmitting SPI message failed");
            }
            gpiod_line_set_value(rckLine, 1);
            gpiod_line_set_value(rckLine, 0);
            gpiod_line_set_value_bulk(&sBulk, &bulkValue[0]);
            tr.tx_buf = (unsigned long)&data[row];
            suc = ioctl(spifd, SPI_IOC_MESSAGE(1), &tr);
            if (suc < 0) {
                pabort("Transmitting SPI message failed");
            }
            gpiod_line_set_value(rckLine, 1);
            gpiod_line_set_value(rckLine, 0);
            nanosleep(&req, NULL);
        }
    }
    return EXIT_SUCCESS;
}
