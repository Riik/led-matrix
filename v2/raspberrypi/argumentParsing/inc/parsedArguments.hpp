#pragma once

#include <cstdint>

struct ParsedArguments {
    uint_fast8_t brightness;
};

constexpr ParsedArguments defaultArguments = {
    .brightness = 7
};
