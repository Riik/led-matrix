#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <signal.h>

#include "matrixDriver.h"
#include "pabort.h"
#include "fontTranslation.h"

static const size_t tileCount = 4;

static atomic_bool halt = false;

static void intHandler(int i) {
    (void)i;
    halt = true;
}

static const char* text = "Woooww, dat is gaaf";

int main(void)
{
    const size_t colCount = tileCount*8;
    struct sigaction act = {
        .sa_handler = intHandler
    };
    sigaction(SIGINT, &act, NULL);
    if (!startLedDriving(colCount)){
        pabort("startLedDriving failed");
    }
    // Translate the text. We need at least as many text tiles as output tiles,
    // so make sure that is the case.
    size_t textTilesCount = strlen(text);
    const size_t textTileArrLen = textTilesCount * TILE_SIZE_ROWS_COLS;
    uint_fast8_t* textTiles = calloc(textTileArrLen, sizeof(uint_fast8_t));
    for (size_t i = 0; i < strlen(text); ++i) {
        const uint_fast8_t* p = charToTile(text[i]);
        memcpy(&textTiles[i * TILE_SIZE_ROWS_COLS], p, TILE_SIZE_BYTE);
    }
    // Prepare the output tiles
    uint_fast8_t* outputTiles = calloc(textTileArrLen, sizeof(textTiles[0]));
    size_t *rowIndex = calloc(tileCount, sizeof(size_t));
    {
        size_t tileStart = 0;
        for (size_t i = 0; i < tileCount; ++i) {
            memcpy(&outputTiles[i*LED_MATRIX_ROW_COUNT], &textTiles[tileStart], LED_MATRIX_ROW_COUNT * sizeof(outputTiles[0]));
            tileStart += LED_MATRIX_ROW_COUNT;
            if (tileStart > textTileArrLen) {
                tileStart = 0;
            }
            rowIndex[i] = tileStart;
        }

    }
    size_t bitIndex = 0;
    unsigned int curBrightness = 100;
    ledDriverSetBrightnessPercentage(curBrightness);

    struct timespec deadline;
    clock_gettime(CLOCK_MONOTONIC, &deadline);
    while(!halt) {
        uint_fast8_t *b = ledDriverGetInactiveBuffer();
        for (size_t row = 0; row < LED_MATRIX_ROW_COUNT; ++row) {
            for (size_t rStart = 0; rStart < tileCount; ++rStart) {
                size_t actTile = tileCount - 1 - rStart;
                b[row * tileCount + rStart] = outputTiles[row + actTile*LED_MATRIX_ROW_COUNT];
            }
        }
        ledDriverSwapBuffer();
        // Update our tile
        for (size_t j = 0; j < tileCount; ++j) {
            for (size_t i = 0; i < LED_MATRIX_ROW_COUNT; ++i) {
                uint_fast8_t newRow = textTiles[rowIndex[j] + i];
                newRow >>= bitIndex;
                outputTiles[i + j*LED_MATRIX_ROW_COUNT] = outputTiles[i + j*LED_MATRIX_ROW_COUNT] >> 1 | newRow << 7;
            }
        }
        bitIndex++;
        if (bitIndex >= LED_MATRIX_ROW_COUNT) {
            bitIndex = 0;
            for (size_t i = 0; i < tileCount; ++i) {
                rowIndex[i] += LED_MATRIX_ROW_COUNT;
                if (rowIndex[i] >= textTileArrLen) {
                    rowIndex[i] = 0;
                }
            }
        }

        deadline.tv_nsec += 40000000;
        if(deadline.tv_nsec >= 1000000000) {
            deadline.tv_nsec -= 1000000000;
            deadline.tv_sec++;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
    }
    // Clear the LED matrix
    uint_fast8_t *b = ledDriverGetInactiveBuffer();
    memset(b, 0, sizeof(b[0])*colCount);
    ledDriverSwapBuffer();

    stopLedDriving();
    free(textTiles);
    free(outputTiles);
    free(rowIndex);
    return EXIT_SUCCESS;
}
