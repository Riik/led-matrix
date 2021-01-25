#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <gpiod.h>

#include "pabort.h"

static const char *spidev = "/dev/spidev0.0";

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
    struct gpiod_line* line = gpiod_chip_get_line(chip, 18);
    if (line == NULL)
        pabort("can't get line from GPIO device");
    int ret = gpiod_line_request_output(line, argv[0], 0);
    if (ret < 0)
        pabort("can't set GPIO 2 to output");

    printf("Hello, world!\n");
    return EXIT_SUCCESS;
}

