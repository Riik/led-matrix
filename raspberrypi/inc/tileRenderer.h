#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

/** These are the objects that can be renderer with this renderer*/
struct RenderingBufferObject {
    ssize_t xLen;           /*The size of the object you want to render in pixels in the horizontal direction*/
    ssize_t yLen;           /*The size of the object you want to render in pixels the vertical direction*/
    ssize_t elemsPerRow;    /*Equal to xLen / 8, rounded up*/
    uint_fast8_t *data;     /*The actual data. This is row based, one bit per pixel, top row first, left to right.*/
};

bool initRenderer(const size_t ledMatrixCount, const char* argv0);

void destroyRenderer(void);

void setViewportCoordinates(ssize_t x, ssize_t y);

void renderBufferObject(struct RenderingBufferObject *obj, ssize_t x, ssize_t y);

void rendererSwapBuffer(void);
#endif //TILE_RENDERER_H
