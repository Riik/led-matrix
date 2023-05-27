#pragma once

#include <cstdint>
#include <string>

enum class SelectedMatrixDriver {
    spi,
    ncurses
};

enum class SelectedSubProgram {
    textScroller,
    diceGenerator
};

struct ParsedArguments {
    uint_fast8_t brightness;
    uint_fast32_t maxFramesPerSecond;
    SelectedMatrixDriver matrixDriver;
    uint_fast16_t ledMatrixWidth;
    uint_fast16_t ledMatrixHeight;
    std::string textScrollerText;
    float textScrollerSpeed;
    uint_fast8_t diceGeneratorSides;
    SelectedSubProgram subProgram;
};
