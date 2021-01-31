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
#include <threads.h>

#include "matrixDriver.h"

/**On the pin layout:
 * SPI:
 *  SPI0 is in use, so that GPIO 10 is MOSI, GPIO11 is SCLK and GPIO8 is CS0 (rck).
 * GPIO:
 *  14: S0
 *  15: S1
 *  18: S2
*/

static unsigned int brightnessPercentage = 100;
static uint_fast8_t* bufActive;
static uint_fast8_t* bufInactive;
static atomic_bool threadStop = false;
static thrd_t driveLedsThread;
static cnd_t swapCond;
static mtx_t swapMutex;

static int thrdRetvalToErrno(const int thrd_ret)
{
    switch (thrd_ret)
    {
        case thrd_busy:
            return EBUSY;
        case thrd_error:
            return EINVAL;
        case thrd_nomem:
            return ENOMEM;
        default:
            return 0;
    }
}

static uint_fast8_t *getActiveBuf(void)
{
    if (mtx_trylock(&swapMutex) == thrd_success) {
        uint_fast8_t *temp = bufActive;
        bufActive = bufInactive;
        bufInactive = temp;
        mtx_unlock(&swapMutex);
        cnd_signal(&swapCond);
    }
    return bufActive;
}

static int driveLeds(void *param)
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
    while(!threadStop) {
        const long totalNsecSleep = 1000000;
        long nsecSleepOn;
        long nsecSleepOff;
        uint_fast8_t* buf = getActiveBuf();
        if (brightnessPercentage == 0) {
            nsecSleepOn = 0;
            nsecSleepOff = totalNsecSleep;
        } else {
            nsecSleepOn = totalNsecSleep / (100 / brightnessPercentage);
            nsecSleepOff = totalNsecSleep - nsecSleepOn;
        }
        for (size_t row = 0; row < LED_MATRIX_ROW_COUNT; ++row) {
            int bulkValue[3] = {row & 0x1 ? 1 : 0, row & 0x2 ? 1 : 0, row & 0x4 ? 1 : 0};
            tr.tx_buf = (unsigned long)zeros;
            ioctl(spifd, SPI_IOC_MESSAGE(1), &tr);
            gpiod_line_set_value_bulk(&sBulk, &bulkValue[0]);
            if (nsecSleepOff != 0) {
                deadline.tv_nsec += nsecSleepOff;
                if (deadline.tv_nsec >= 1000000000) {
                    deadline.tv_nsec -= 1000000000;
                    deadline.tv_sec++;
                }
                clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
                if (nsecSleepOn == 0)
                    continue;
            }
            tr.tx_buf = (unsigned long)&buf[row*(colCount / LED_MATRIX_ROW_COUNT)];
            ioctl(spifd, SPI_IOC_MESSAGE(1), &tr);
            deadline.tv_nsec += nsecSleepOn;
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
    return 0;
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
    int suc;
    if ((suc = cnd_init(&swapCond)) != thrd_success) {
        errno = thrdRetvalToErrno(suc);
        goto startLedDriving_bufInactive;
    }
    if ((suc = mtx_init(&swapMutex, mtx_plain)) != thrd_success) {
        errno = thrdRetvalToErrno(suc);
        goto startLedDriving_destroyCond;
    }
    if ((suc = mtx_lock(&swapMutex)) != thrd_success) {
        errno = thrdRetvalToErrno(suc);
        goto startLedDriving_destroyMtx;
    }
    if ((suc = thrd_create(&driveLedsThread, driveLeds, (void*)colCount)) != thrd_success) {
        errno = thrdRetvalToErrno(suc);
        goto startLedDriving_destroyMtx;
    }
    return true;
startLedDriving_destroyMtx:
    mtx_destroy(&swapMutex);
startLedDriving_destroyCond:
    cnd_destroy(&swapCond);
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
    thrd_join(driveLedsThread, NULL);
    mtx_destroy(&swapMutex);
    cnd_destroy(&swapCond);
    free(bufInactive);
    free(bufActive);
}

void ledDriverSwapBuffer(void)
{
    cnd_wait(&swapCond, &swapMutex);
}

uint_fast8_t *ledDriverGetInactiveBuffer(void)
{
    return bufInactive;
}

bool ledDriverSetBrightnessPercentage(unsigned int newBrightnessPercentage) {
    if (newBrightnessPercentage > 100)
        return false;
   brightnessPercentage = newBrightnessPercentage;
   return true;
}
