#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void spi_init(uint32_t speed_hz, bool cpol, bool cpha);

void spi_transaction(uint8_t* txBuf, size_t txBufLen, uint8_t* rxBuf, size_t rxBufLen);

#ifdef __cplusplus
} /* extern "C" */
#endif
