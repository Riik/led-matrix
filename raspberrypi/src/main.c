#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <signal.h>

#include "matrixDriver.h"
#include "pabort.h"
#include "fontTranslation.h"

static const size_t ledMatrixCount = 4;
static atomic_bool halt = false;

static void intHandler(int i) {
    (void)i;
    halt = true;
}

static const char* text = "Woooww, dat is gaaf";

int main(int argc, const char **argv)
{
    const ssize_t totalLedMatrixPixels = ledMatrixCount * 8;
    struct sigaction act = {
        .sa_handler = intHandler
    };
    sigaction(SIGINT, &act, NULL);
    if (!initRenderer(ledMatrixCount, argv[0])){
        pabort("startLedDriving failed");
    }
    struct RenderingBufferObject* tRbo = textToRBO(text, strlen(text));

    unsigned int curBrightness = 100;
    ledDriverSetBrightnessPercentage(curBrightness);

    struct timespec deadline;
    clock_gettime(CLOCK_MONOTONIC, &deadline);
    ssize_t viewPortX = -totalLedMatrixPixels;
    while(!halt) {
        setViewportCoordinates(viewPortX, 0);
        renderBufferObject(tRbo, 0, 0);
        rendererSwapBuffer();
        deadline.tv_nsec += 40000000;
        if(deadline.tv_nsec >= 1000000000) {
            deadline.tv_nsec -= 1000000000;
            deadline.tv_sec++;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
        viewPortX++;
        if (viewPortX >= tRbo->xLen) {
            viewPortX = -totalLedMatrixPixels;
        }
    }
    // Clear the LED matrix
    ledDriverSwapBuffer();

    destroyTextRBO(tRbo);
    destroyRenderer();
    return EXIT_SUCCESS;
}
