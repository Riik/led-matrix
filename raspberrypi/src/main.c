#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <signal.h>

#include "matrixDriver.h"
#include "pabort.h"

const size_t rows = 8;
atomic_bool halt = false;

static void intHandler(int i) {
    (void)i;
    halt = true;
}

int main(void)
{
    struct sigaction act = {
        .sa_handler = intHandler
    };
    sigaction(SIGINT, &act, NULL);
    if (!startLedDriving(rows)){
        pabort("startLedDriving failed");
    }
    struct timespec deadline;
    clock_gettime(CLOCK_MONOTONIC, &deadline);
    while(!halt) {
        for (size_t i = 0; i < 2; ++i) {
            uint_fast8_t *b = ledDriverGetInactiveBuffer();
            int val = i == 0 ? 0 : 255;
            memset(b, val, rows * sizeof(b[0]));
            ledDriverSwapBuffer();
            deadline.tv_sec += 1;
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
        }
    }
    stopLedDriving();
    return EXIT_SUCCESS;
}
