#pragma once

#include "texture2d.hpp"

namespace Gfx2D {
    // Note that the fontIndex shall not exceed 255, otherwise this function will throw

    struct FontColorMap {
        PixelColor oneMap;
        PixelColor zeroMap;
    };

    Texture fontToTexture(std::size_t fontIndex, FontColorMap colorMap = {PixelColor::on, PixelColor::off},
            Texture::WrapMode wrapModeS = Texture::WrapMode::ClampToBorder, Texture::WrapMode wrapModeT = Texture::WrapMode::ClampToBorder,
            PixelColor borderColor = PixelColor::on) noexcept(false);

    std::size_t getMaxFontIndex() noexcept;
}
