#include <algorithm>
#include <stdexcept>
#include <sstream>

#include "matrixScreen.hpp"

MatrixScreen::MatrixScreen(const std::size_t& matrixCountWidth, const std::size_t& matrixCountHeight) : matrixCountWidth{matrixCountWidth},
    matrixCountHeight{matrixCountHeight}, pixelCountWidth{matrixCountWidth*matrixPixelCountWidth},
    pixelCountHeight{matrixCountHeight*matrixPixelCountHeight}
{
    this->screen = std::vector<PixelColor>(this->pixelCountWidth*this->pixelCountHeight,
            PixelColor::off);
}

void MatrixScreen::resetScreen(PixelColor color)
{
    std::fill(this->screen.begin(), this->screen.end(), color);
}


PixelColor MatrixScreen::operator()(std::size_t width, std::size_t height) const
{
    if (width >= this->pixelCountWidth) {
        width = this->pixelCountWidth - 1;
    }
    if (height >= this->pixelCountHeight) {
        height = this->pixelCountHeight - 1;
    }
    return this->screen.at(this->pixelCountWidth*height + width);
}


PixelColor& MatrixScreen::operator()(std::size_t width, std::size_t height)
{
    if (width >= this->pixelCountWidth) {
        width = this->pixelCountWidth - 1;
    }
    if (height >= this->pixelCountHeight) {
        height = this->pixelCountHeight - 1;
    }
    return this->screen.at(this->pixelCountWidth*height + width);
}

std::size_t MatrixScreen::getMatrixCountWidth() const
{
    return this->matrixCountWidth;
}

std::size_t MatrixScreen::getMatrixCountHeight() const
{
    return this->matrixCountHeight;
}

std::size_t MatrixScreen::getPixelCountWidth() const
{
    return this->pixelCountWidth;
}

std::size_t MatrixScreen::getPixelCountHeight() const
{
    return this->pixelCountHeight;
}

bool MatrixScreen::operator==(const MatrixScreen& other) const
{
    return this->matrixCountWidth == other.matrixCountWidth &&
        this->matrixCountHeight == other.matrixCountHeight &&
        this->screen == other.screen;
}

bool MatrixScreen::operator!=(const MatrixScreen& other) const
{
    return !this->operator==(other);
}
