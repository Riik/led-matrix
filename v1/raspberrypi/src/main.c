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

    struct RenderingBufferObject littleCube;
    littleCube.data = calloc(2, sizeof(*littleCube.data));
    littleCube.xLen = 2;
    littleCube.yLen = 2;
    littleCube.elemsPerRow = 1;
    memset(littleCube.data, 0xff, sizeof(*littleCube.data)*2);

    struct RenderingBufferObject bigCube;
    bigCube.data = calloc(8, sizeof(*bigCube.data));
    bigCube.xLen = 8;
    bigCube.yLen = 8;
    bigCube.elemsPerRow = 1;
    memset(bigCube.data, 0xff, sizeof(*bigCube.data)*8);

    unsigned int curBrightness = 100;
    ledDriverSetBrightnessPercentage(curBrightness);

    struct timespec deadline;
    clock_gettime(CLOCK_MONOTONIC, &deadline);
    ssize_t viewPortX = 0;
    ssize_t viewPortY = 0;
    bool reverse = false;
    bool yReverse = true;
    while(!halt) {
        setViewportCoordinates(viewPortX, viewPortY);
        renderBufferObject(&littleCube, 6, 0);
        renderBufferObject(&littleCube, 5, 2);
        renderBufferObject(&littleCube, 4, 4);
        renderBufferObject(&littleCube, 3, 6);
        renderBufferObject(&littleCube, 2, 4);
        renderBufferObject(&littleCube, 1, 2);
        renderBufferObject(&littleCube, 0, 0);

        renderBufferObject(&bigCube, 16, 0);
        renderBufferObject(&bigCube, -16, 0);
        rendererSwapBuffer();
        deadline.tv_nsec += 40000000;
        while (deadline.tv_nsec >= 1000000000) {
            deadline.tv_nsec -= 1000000000;
            deadline.tv_sec++;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
        if (reverse)
            viewPortX--;
        else
            viewPortX++;
        if (viewPortX >= littleCube.xLen) {
            reverse = true;
        } else if (viewPortX <= -totalLedMatrixPixels + 7) {
            reverse = false;
        }

        if (yReverse) {
            viewPortY--;
        } else {
            viewPortY++;
        }

        if (viewPortY >= 3) {
            yReverse = true;
        } else if (viewPortY <= -3) {
            yReverse = false;
        }

    }
    // Clear the LED matrix
    ledDriverSwapBuffer();

    destroyTextRBO(tRbo);
    destroyRenderer();
    free(littleCube.data);
    free(bigCube.data);
    return EXIT_SUCCESS;
}
