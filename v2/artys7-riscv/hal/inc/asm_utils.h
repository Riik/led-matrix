#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t getCycleCount(void);
uint64_t getSystemTimeUs(void);
uint64_t getInstructionsRetiredCount(void);

#ifdef __cplusplus
} /* extern "C" */
#endif
