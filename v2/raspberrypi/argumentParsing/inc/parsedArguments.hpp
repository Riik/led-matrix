#pragma once

#include <cstdint>

enum class SelectedMatrixDriver {
    spi,
    ncurses
};

struct ParsedArguments {
    uint_fast8_t brightness;
    uint_fast32_t maxFramesPerSecond;
    SelectedMatrixDriver matrixDriver;
};

constexpr ParsedArguments defaultArguments = {
    .brightness = 7,
    .maxFramesPerSecond = 200,
#if defined(__arm__) || defined(__aarch64__)
    .matrixDriver = SelectedMatrixDriver::spi,
#else
    .matrixDriver = SelectedMatrixDriver::ncurses,
#endif //defined(__arm__) || defined(__aarch64__)
};
