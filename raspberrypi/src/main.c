#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <signal.h>

#include "matrixDriver.h"
#include "pabort.h"
#include "fontTranslation.h"

static const size_t colCount = 8;
atomic_bool halt = false;

static void intHandler(int i) {
    (void)i;
    halt = true;
}

const char* text = "Dag amice Van der Vlist";
const char* text2 = ". Met mij gaat het prima. Hoe maakt u het? ";

int main(void)
{
    struct sigaction act = {
        .sa_handler = intHandler
    };
    sigaction(SIGINT, &act, NULL);
    if (!startLedDriving(colCount)){
        pabort("startLedDriving failed");
    }
    // Translate the text
    uint_fast8_t* tiles = calloc(strlen(text) + strlen(text2) + 1, TILE_SIZE_BYTE);
    const size_t colCount = (strlen(text) + strlen(text2) + 1) *8;
    for (size_t i = 0; i < strlen(text); ++i) {
        const uint_fast8_t* p = charToTile(text[i]);
        memcpy(&tiles[i * TILE_SIZE_ROWS_COLS], p, TILE_SIZE_BYTE);
    }
    // Keep p out of scope
    {
        const uint_fast8_t* p = charToTile(7);
        memcpy(&tiles[strlen(text) * TILE_SIZE_ROWS_COLS], p, TILE_SIZE_BYTE);
    }
    for (size_t i = 0; i < strlen(text2); ++i) {
        const uint_fast8_t* p = charToTile(text2[i]);
        memcpy(&tiles[(strlen(text) + 1 + i) * TILE_SIZE_ROWS_COLS], p, TILE_SIZE_BYTE);
    }
    // Prepare one tile
    uint_fast8_t* tile = calloc(colCount, sizeof(tiles[0]));
    memcpy(tile, tiles, colCount*sizeof(tiles[0]));
    size_t rowIndex = 8;
    size_t bitIndex = 0;

    struct timespec deadline;
    clock_gettime(CLOCK_MONOTONIC, &deadline);
    while(!halt) {
        uint_fast8_t *b = ledDriverGetInactiveBuffer();
        memcpy(b, tile, sizeof(b[0])*8);
        ledDriverSwapBuffer();
        // Update our tile
        // TODO fix this for colcount > 8
        for (size_t i = 0; i < LED_MATRIX_ROW_COUNT; ++i) {
            uint_fast8_t newRow = tiles[rowIndex + i];
            newRow >>= bitIndex;
            tile[i] = tile[i] >> 1 | newRow << 7;
        }
        bitIndex++;
        if (bitIndex >= 8) {
            bitIndex = 0;
            rowIndex += 8;
            if (rowIndex >= colCount * 8)
                rowIndex = 0;
        }
        deadline.tv_nsec += 40000000;
        if(deadline.tv_nsec >= 1000000000) {
            deadline.tv_nsec -= 1000000000;
            deadline.tv_sec++;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
    }
    stopLedDriving();
    free(tiles);
    free(tile);
    return EXIT_SUCCESS;
}
