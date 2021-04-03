#include <string.h>
#include <threads.h>

#include "tileRenderer.h"
#include "matrixDriver.h"
#include "thrdRetvalToErrno.h"

static mtx_t tileRendererMutex;
static ssize_t viewPortX;
static ssize_t viewPortY;
static uint_fast8_t *renderBuf;
static size_t matrixCount;

bool initRenderer(const size_t ledMatrixCount, const char* argv0)
{
    if (!startLedDriving(ledMatrixCount, argv0)) {
        return false;
    }
    int suc;
    if ((suc = mtx_init(&tileRendererMutex, mtx_plain)) != thrd_success) {
        stopLedDriving();
        errno = thrdRetvalToErrno(suc);
        return false;
    }
    viewPortX = 0;
    viewPortY = 0;
    matrixCount = ledMatrixCount;
    renderBuf = ledDriverGetInactiveBuffer();
    memset(renderBuf, 0, LED_MATRIX_COL_COUNT_PER_MATRIX*ledMatrixCount*sizeof(renderBuf[0]));
    return true;
}

void destroyRenderer(void)
{
    stopLedDriving();
    mtx_destroy(&tileRendererMutex);
}

void rendererSwapBuffer(void)
{
    ledDriverSwapBuffer();
    renderBuf = ledDriverGetInactiveBuffer();
    memset(renderBuf, 0, matrixCount*LED_MATRIX_COL_COUNT_PER_MATRIX*sizeof(renderBuf[0]));
}

void setViewportCoordinates(ssize_t x, ssize_t y)
{
    viewPortX = x;
    viewPortY = y;
}

void renderBufferObject(struct RenderingBufferObject *obj, ssize_t x, ssize_t y)
{
    const ssize_t viewPortLen = matrixCount*LED_MATRIX_ROW_COUNT - 1;
    ssize_t viewPortXStart = viewPortX;
    ssize_t viewPortXEnd = viewPortX + viewPortLen;
    ssize_t objectXStart = x;
    ssize_t objectXEnd = x + obj->xLen;
    // Check if the object is even in view. If not, then skip it.
    if (objectXEnd < viewPortXStart || objectXStart > viewPortXEnd)
        return;

    // It is in view! Check what exactly is in view
    // First question: is there an empty space preceeding?
    ssize_t emptyPrefixPixels = 0;
    if (objectXStart > viewPortXStart) {
        emptyPrefixPixels = objectXStart - viewPortXStart;
    }

    // Which pixel is the first one that should be displayed?
    ssize_t objectViewXStart;
    if (objectXStart >= viewPortXStart) {
        objectViewXStart = 0;
    } else {
        objectViewXStart = viewPortXStart - objectXStart;
    }
    size_t objectViewXStartElem = objectViewXStart / 8;
    size_t objectViewXCurrentElem = objectViewXStartElem;
    size_t objectViewXStartPixel = objectViewXStart % 8;
    size_t objectViewXCurrentPixel = objectViewXStartPixel;

    size_t firstNonEmptyMatrix = emptyPrefixPixels / 8;
    size_t firstMatrixEmptyPixels = emptyPrefixPixels % 8;

    for (size_t j = 0; j < 8; ++j) {
        for (size_t i = firstNonEmptyMatrix; i < matrixCount; ++i) {
            if (objectViewXCurrentElem >= obj->elemsPerRow)
                break;
            size_t pixelsDrawn = 0;
            // Load the current element
            uint_fast8_t elem = obj->data[j*obj->elemsPerRow + objectViewXCurrentElem];
            // Shift it to the right to correct for current pixel
            elem >>= objectViewXCurrentPixel;
            pixelsDrawn += 8 - objectViewXCurrentPixel;
            objectViewXCurrentElem++;
            if (pixelsDrawn < 8 && objectViewXCurrentElem < obj->elemsPerRow) {
                uint_fast8_t otherElem = obj->data[j*obj->elemsPerRow + objectViewXCurrentElem];
                otherElem <<= pixelsDrawn;
                elem |= otherElem;

            }
            if (i == firstNonEmptyMatrix) {
                // Shift it to the left to correct for empty pixels
                elem <<= firstMatrixEmptyPixels;
                if (firstMatrixEmptyPixels > objectViewXCurrentPixel) {
                    objectViewXCurrentPixel += 8 - firstMatrixEmptyPixels;
                    objectViewXCurrentElem--;
                } else {
                    objectViewXCurrentPixel -= firstMatrixEmptyPixels;
                }

            }
            renderBuf[j*matrixCount + (matrixCount - 1 - i)] = elem;
        }
        objectViewXCurrentElem = objectViewXStartElem;
        objectViewXCurrentPixel = objectViewXStartPixel;
    }
}
