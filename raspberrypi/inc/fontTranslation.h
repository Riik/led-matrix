#ifndef FONT_TRANSLATION_H
#define FONT_TRANSLATION_H

#include <stdint.h>

#define TILE_SIZE_BYTE (sizeof(uint_fast8_t)*8)
#define TILE_SIZE_ROWS_COLS 8

const uint_fast8_t *charToTile(const char c);

#endif
