#include <cmath>
#include <stdexcept>

#include "texture2d.hpp"

Gfx2D::Texture::Texture(std::vector<PixelColor> texels, std::size_t texelWidth, std::size_t texelHeight,
        Gfx2D::Texture::WrapMode wrapModeS, Gfx2D::Texture::WrapMode wrapModeT, PixelColor borderColor) :
    texels(std::move(texels)), texelWidth(texelWidth), texelHeight(texelHeight), wrapModeS(wrapModeS),
    wrapModeT(wrapModeT), borderColor(borderColor) {
        if (texelWidth * texelHeight != this->texels.size()) {
            throw std::invalid_argument("width times height does not match vector size");
        }

        if (texelWidth * texelHeight == 0) {
            throw std::invalid_argument("Total texel count cannot be zero");
        }
    }

static std::size_t inboundFloatToTexelCoordinate(float f, std::size_t numTexels)
{
        return (std::size_t)(floor(f * numTexels));
}

static std::size_t getTexelCoordinate(float f, std::size_t numTexels, Gfx2D::Texture::WrapMode wrapMode)
{
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    if (f >= 0 && f < 1) {
        return inboundFloatToTexelCoordinate(f, numTexels);
    }

    switch (wrapMode) {
        case Gfx2D::Texture::WrapMode::Repeat:
            {
                float actF = f - floorf(f);
                // Because floats are hard, this calculation could result in actF being 1.0f.
                // This happens, for example, if f is the largest possible value below zero (say -delta)
                // Now floor(f) equals -1, so f - floor(f) should logcally be equivalient to 1 - delta.
                // 1 - delta does not fit in a float, so the the outcome is actually 1.
                // Check for this
                if (actF == 1.0f) {
                    actF -= epsilon;
                }
                return inboundFloatToTexelCoordinate(actF, numTexels);
            }
        case Gfx2D::Texture::WrapMode::MirroredRepeat:
            {
                float actF = f - floorf(f);
                // In the even case we just repeat and in the odd case we mirror
                if (static_cast<int>(floor(f)) % 2 != 0) {
                    actF = 1 - actF;
                }
                // Because floats are hard, this calculation could result in actF being 1.0f. Check for this
                if (actF == 1.0f) {
                    actF -= epsilon;
                }
                return inboundFloatToTexelCoordinate(actF, numTexels);
            }
        case Gfx2D::Texture::WrapMode::ClampToEdge:
            if (f < 0) {
                return 0;
            } else {
                return numTexels - 1;
            }
        case Gfx2D::Texture::WrapMode::ClampToBorder:
            return numTexels;
    }

    throw std::invalid_argument("Unknown wrap mode");
}

PixelColor Gfx2D::Texture::getColorAt(float s, float t) const
{
    std::size_t x = getTexelCoordinate(s, this->texelWidth, this->wrapModeS);
    std::size_t y = getTexelCoordinate(t, this->texelHeight, this->wrapModeT);
    if (x == this->texelWidth || y == this->texelHeight) {
        return this->borderColor;
    }
    return this->texels[this->texelWidth*y + x];
}
