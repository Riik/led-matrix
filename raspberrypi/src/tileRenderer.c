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

    ssize_t viewPortYStart = viewPortY;
    ssize_t viewPortYEnd = viewPortY + LED_MATRIX_ROW_COUNT;
    ssize_t objectYStart = y;
    ssize_t objectYEnd = y + obj->yLen;

    if (objectYEnd < viewPortYStart || objectYStart > viewPortYEnd)
        return;

    // It is in view! Check what exactly is in view
    // First, handle the x part
    // First question: is there an empty space preceeding?
    size_t emptyPrefixPixels = 0;
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
    const size_t objectViewXStartElem = objectViewXStart / 8;
    size_t objectViewXCurrentElem = objectViewXStartElem;
    const size_t objectViewXStartPixel = objectViewXStart % 8;
    size_t objectViewXCurrentPixel = objectViewXStartPixel;

    const size_t firstNonEmptyMatrix = emptyPrefixPixels / 8;
    const size_t firstMatrixEmptyPixels = emptyPrefixPixels % 8;

    size_t finalElementMaskLen = obj->elemsPerRow*8 - obj->xLen;
    uint_fast8_t finalElementMask = 0;
    if (finalElementMaskLen > 0) {
        finalElementMask = (1 << (8 - finalElementMaskLen)) - 1;
    }

    // Now, handle the y part.
    size_t emptyPrefixLines = 0;
    if (objectYEnd < viewPortYEnd) {
        emptyPrefixLines = viewPortYEnd - objectYEnd;
    }
    size_t objectStartLine = 0;
    if (objectYEnd > viewPortYEnd) {
        objectStartLine = objectYEnd - viewPortYEnd;
    }
    size_t objectEndLine = obj->yLen;

    for (size_t j = emptyPrefixLines; j < 8; ++j) {
        for (size_t i = firstNonEmptyMatrix; i < matrixCount && objectViewXCurrentElem < obj->elemsPerRow; ++i) {
            size_t pixelsDrawn = 0;
            // Load the current element
            uint_fast8_t elem = obj->data[objectStartLine*obj->elemsPerRow + objectViewXCurrentElem];
            if (objectViewXCurrentElem + 1 >= obj->elemsPerRow) {
                elem &= finalElementMask;
            }
            // Shift it to the right to correct for current pixel
            elem >>= objectViewXCurrentPixel;
            pixelsDrawn += 8 - objectViewXCurrentPixel;
            objectViewXCurrentElem++;
            if (pixelsDrawn < 8 && objectViewXCurrentElem < obj->elemsPerRow) {
                uint_fast8_t otherElem = obj->data[objectStartLine*obj->elemsPerRow + objectViewXCurrentElem];
                if (objectViewXCurrentElem + 1 >= obj->elemsPerRow) {
                    otherElem &= finalElementMask;
                }
                otherElem <<= pixelsDrawn;
                elem |= otherElem;
            }

            // If it is the first or last element, we might have to merge with existing data.
            if (i == firstNonEmptyMatrix) {
                // Shift the element to correct for empty pixels
                elem <<= firstMatrixEmptyPixels;
                if (firstMatrixEmptyPixels > objectViewXCurrentPixel && firstMatrixEmptyPixels + obj->xLen > 8) {
                    objectViewXCurrentPixel += 8 - firstMatrixEmptyPixels;
                    objectViewXCurrentElem--;
                } else {
                    objectViewXCurrentPixel -= firstMatrixEmptyPixels;
                }
                uint_fast8_t mask = ~(255 << firstMatrixEmptyPixels);
                // Merge with existing element
                uint_fast8_t oldElem = renderBuf[j*matrixCount + (matrixCount - 1 - i)] & mask;
                renderBuf[j*matrixCount + (matrixCount - 1 - i)] = elem | oldElem;
            } else if (objectViewXCurrentElem >= obj->elemsPerRow) {
                uint_fast8_t mask = finalElementMask;
                mask >>= objectViewXCurrentPixel;
                uint_fast8_t oldElem = renderBuf[j*matrixCount + (matrixCount - 1 - i)];
                oldElem &= ~mask;
                renderBuf[j*matrixCount + (matrixCount - 1 - i)] = elem | oldElem;
            } else {
                renderBuf[j*matrixCount + (matrixCount - 1 - i)] = elem;
            }
        }
        objectViewXCurrentElem = objectViewXStartElem;
        objectViewXCurrentPixel = objectViewXStartPixel;
        objectStartLine += 1;
        if (objectStartLine >= objectEndLine) {
            break;
        }
    }
}
