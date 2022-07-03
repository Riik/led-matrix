#pragma once

#include <vector>

#include "pixelColor.hpp"

namespace Gfx2D {
    /* A texture is stored row based: 1, 0 is the second element of the vector */
    class Texture {
        public:
            enum class WrapMode {
                Repeat,
                MirroredRepeat,
                ClampToEdge,
                ClampToBorder
            };
            Texture(std::vector<PixelColor> texels, std::size_t texelWidth, std::size_t texelHeight,
                    WrapMode wrapModeS = WrapMode::Repeat, WrapMode wrapModeT = WrapMode::Repeat,
                    PixelColor borderColor = PixelColor::on);

            PixelColor getColorAt(float s, float t) const;
        private:
            std::vector<PixelColor> texels;
            std::size_t texelWidth, texelHeight;
            WrapMode wrapModeS, wrapModeT;
            PixelColor borderColor;
    };
}
