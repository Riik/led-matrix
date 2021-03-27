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
    if (objectXStart > viewPortXStart && objectXEnd < viewPortXEnd)
        return;
    
    // It is in view! Check what exactly is in view
    ssize_t emptyPrefixPixels = 0;
    if (objectXStart > viewPortXStart) {
        emptyPrefixPixels = objectXStart - viewPortXStart;
    }
    ssize_t objectViewXStart;
    if (objectXStart <= viewPortXStart) {
        objectViewXStart = 0;
    } else {
        objectViewXStart = objectXStart - viewPortXStart;
    }
    size_t objectViewXStartElem = objectViewXStart / 8;
    size_t objectViewXElem = objectViewXStartElem;
    size_t objectViewXShift = objectViewXStart % 8;
    size_t viewStartRow = 0;
    if (obj->yLen < 8) {
        viewStartRow = 8 - obj->yLen;
    }
    size_t curObjRow = 0;
    
    for (size_t j = viewStartRow; j < 8; ++j) {
        for (size_t i = 0; i < matrixCount; ++i) {
            if (objectViewXElem >= obj->elemsPerRow)
                continue;
            uint_fast8_t elem = obj->data[curObjRow*obj->elemsPerRow + objectViewXElem];
            elem >>= objectViewXShift;
            objectViewXElem++;
            if (objectViewXShift != 0 && objectViewXElem < obj->elemsPerRow) {
                uint_fast8_t otherElem = obj->data[curObjRow*obj->elemsPerRow + objectViewXElem];
                otherElem <<= (8 - objectViewXShift);
                elem |= otherElem;
            }
            renderBuf[j*matrixCount + (matrixCount - 1 - i)] = elem;
        }
        curObjRow++;
        objectViewXElem = objectViewXStartElem;
    }
}
