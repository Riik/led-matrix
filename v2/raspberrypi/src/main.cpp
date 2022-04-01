#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

const char *spidev = "/dev/spidev0.0";

int main(void) {
    int spifd = open(spidev, O_RDWR);
    if (spifd < 0) {
        fprintf(stderr, "open %s failed\n", spidev);
        return EXIT_FAILURE;
    }

    struct spi_ioc_transfer tr[10] = {{0}};
    uint8_t data[20];
    for (size_t i = 0; i < sizeof(tr)/sizeof(tr[0]); i += 1) {
        tr[i].bits_per_word = 8;
        tr[i].speed_hz = 10000000;
        tr[i].len = 2;
        tr[i].tx_buf = (unsigned long)&data[i*2];
        tr[i].cs_change = 1;
    }
    tr[(sizeof(tr)/sizeof(tr[0])) - 1].cs_change = 0;
    // Scan all lines
    data[0] = 0x0B;
    data[1] = 0x07;
    // Intensity max
    data[4] = 0x0A;
    data[5] = 0x0F;
    // Draw a neat line
    data[2] = 0x01;
    data[3] = 0x80;
    data[4] = 0x02;
    data[5] = 0x01;
    data[6] = 0x03;
    data[7] = 0x02;
    data[8] = 0x04;
    data[9] = 0x04;
    data[10] = 0x05;
    data[11] = 0x08;
    data[12] = 0x06;
    data[13] = 0x10;
    data[14] = 0x07;
    data[15] = 0x20;
    data[16] = 0x08;
    data[17] = 0x40;
    // Disable shutdown
    data[18] = 0x0C;
    data[19] = 0x01;
    // Transmit
    ioctl(spifd, SPI_IOC_MESSAGE(sizeof(tr)/sizeof(tr[0])), &tr[0]);

    close(spifd);
    return EXIT_SUCCESS;
}
