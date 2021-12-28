#ifndef FONT_TRANSLATION_H
#define FONT_TRANSLATION_H

#include <stdint.h>
#include "tileRenderer.h"

#define TILE_SIZE_BYTE (sizeof(uint_fast8_t)*8)
#define TILE_SIZE_ROWS_COLS 8

const uint_fast8_t *charToTile(const char c);

struct RenderingBufferObject *textToRBO(const char* c, const size_t strlen);

void destroyTextRBO(struct RenderingBufferObject* rbo);

#endif
