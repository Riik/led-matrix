#pragma once

#include <cstdint>

struct ParsedArguments {
    uint_fast8_t brightness;
    uint_fast32_t maxFramesPerSecond;
};

constexpr ParsedArguments defaultArguments = {
    .brightness = 7,
    .maxFramesPerSecond = 200
};
