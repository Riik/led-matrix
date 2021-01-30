#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdatomic.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <gpiod.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <linux/spi/spidev.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#include "matrixDriver.h"

/**On the pin layout:
 * SPI:
 *  SPI0 is in use, so that GPIO 10 is MOSI, GPIO11 is SCLK and GPIO8 is CS0 (rck).
 * GPIO:
 *  14: S0
 *  15: S1
 *  18: S2
*/

static uint_fast8_t* bufActive;
static uint_fast8_t* bufInactive;
static atomic_bool threadStop = false;
static atomic_bool threadInit = false;
static pthread_t driveLedsThread;
static sem_t swapRequestSem;
static sem_t swapOkSem;

static uint_fast8_t *getActiveBuf(void)
{
    if (sem_trywait(&swapRequestSem) == 0) {
        uint_fast8_t *temp = bufActive;
        bufActive = bufInactive;
        bufInactive = temp;
        sem_post(&swapOkSem);
    }
    return bufActive;
}

static void *driveLeds(void *param)
{
    const char *spidev = "/dev/spidev0.0";
    unsigned int sLineOffsets[] = {14, 15, 18};
    const int sLineDefaults[] = {0, 0, 0};
    size_t colCount = (size_t)param;
    int spifd = open(spidev, O_RDWR);
    if (spifd < 0) {
        fprintf(stderr, "Opening spidev failed");
        goto driveLeds_exit;
    }
    struct gpiod_chip* chip = gpiod_chip_open_by_name("gpiochip0");
    if (chip == NULL) {
        fprintf(stderr, "Opening gpiochip0 failed");
        goto driveLeds_closeSpi;
    }
    //Get S0, S1 and S2 in bulk
    struct gpiod_line_bulk sBulk;
    int suc = gpiod_chip_get_lines(chip, &sLineOffsets[0], sizeof(sLineOffsets)/sizeof(sLineOffsets[0]), &sBulk);
    if (suc != 0) {
        fprintf(stderr, "Getting the S-lines as bulk failed");
        goto driveLeds_closeChip;
    }
    // Set the GPIO lines to output and 0
    suc = gpiod_line_request_bulk_output(&sBulk, "prog", &sLineDefaults[0]);
    if (suc != 0) {
        fprintf(stderr, "Setting the gpio lines as output to 0 failed");
        goto driveLeds_releaseBulk;
    }
    // Prepare the SPI output
    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));
    tr.len = colCount / LED_MATRIX_ROW_COUNT;
    tr.speed_hz = 50000000;
    tr.bits_per_word = 8;
    struct timespec deadline;
    clock_gettime(CLOCK_MONOTONIC, &deadline);
    uint_fast8_t *zeros = calloc(colCount / LED_MATRIX_ROW_COUNT, sizeof(uint_fast8_t));
    if (zeros == NULL) {
        fprintf(stderr, "Allocating zeros array failed");
        goto driveLeds_releaseBulk;
    }
    threadInit = true;
    while(!threadStop) {
        for (size_t row = 0; row < LED_MATRIX_ROW_COUNT; ++row) {
            uint_fast8_t* buf = getActiveBuf();
            int bulkValue[3] = {row & 0x1 ? 1 : 0, row & 0x2 ? 1 : 0, row & 0x4 ? 1 : 0};
            tr.tx_buf = (unsigned long)zeros;
            ioctl(spifd, SPI_IOC_MESSAGE(1), &tr);
            gpiod_line_set_value_bulk(&sBulk, &bulkValue[0]);
            tr.tx_buf = (unsigned long)&buf[row*(colCount / LED_MATRIX_ROW_COUNT)];
            ioctl(spifd, SPI_IOC_MESSAGE(1), &tr);
            deadline.tv_nsec += 1000000;
            if(deadline.tv_nsec >= 1000000000) {
                deadline.tv_nsec -= 1000000000;
                deadline.tv_sec++;
            }
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
        }
    }
    free(zeros);
driveLeds_releaseBulk:
    gpiod_line_release_bulk(&sBulk);
driveLeds_closeChip:
    gpiod_chip_close(chip);
driveLeds_closeSpi:
    close(spifd);
driveLeds_exit:
    return NULL;
}

bool startLedDriving(size_t colCount)
{
    if (colCount == 0 || colCount % LED_MATRIX_ROW_COUNT != 0) {
        errno = EINVAL;
        goto startLedDriving_err;
    }
    bufActive = calloc(colCount, sizeof(uint_fast8_t));
    if (bufActive == NULL) {
        goto startLedDriving_err;
    }
    bufInactive = calloc(colCount, sizeof(uint_fast8_t));
    if (bufInactive == NULL) {
        goto startLedDriving_bufActive;
    }
    if (sem_init(&swapRequestSem, 0, 0) == -1) {
        goto startLedDriving_bufInactive;
    }
    if (sem_init(&swapOkSem, 0, 0) == -1) {
        goto startLedDriving_requestSem;
    }

    int err = pthread_create(&driveLedsThread, NULL, driveLeds, (void*)colCount);
    if (err != 0) {
        errno = err;
        goto startLedDriving_okSem;
    }
    while(!threadInit) {
        if (pthread_kill(driveLedsThread, 0) == ESRCH) {
            errno = ESRCH;
            goto startLedDriving_okSem;
        }
    }
    return true;
startLedDriving_okSem:
    sem_destroy(&swapOkSem);
startLedDriving_requestSem:
    sem_destroy(&swapRequestSem);
startLedDriving_bufInactive:
    free(bufInactive);
startLedDriving_bufActive:
    free(bufActive);
startLedDriving_err:
    return false;
}

void stopLedDriving(void)
{
    threadStop = true;
    pthread_join(driveLedsThread, NULL);
    free(bufActive);
    free(bufInactive);
    sem_destroy(&swapOkSem);
    sem_destroy(&swapRequestSem);
}

void ledDriverSwapBuffer(void)
{
    sem_post(&swapRequestSem);
    sem_wait(&swapOkSem);
}

uint_fast8_t *ledDriverGetInactiveBuffer(void)
{
    return bufInactive;
}
