#include <algorithm>
#include <stdexcept>
#include <sstream>

#include "matrixScreen.hpp"

MatrixScreen::MatrixScreen(const std::size_t& matrixCountWidth, const std::size_t& matrixCountHeight) : matrixCountWidth{matrixCountWidth},
    matrixCountHeight{matrixCountHeight}, pixelCountWidth{matrixCountWidth*matrixPixelCountWidth},
    pixelCountHeight{matrixCountHeight*matrixPixelCountHeight}
{
    this->screen = std::vector<MatrixScreen::PixelColor>(this->pixelCountWidth*this->pixelCountHeight,
            MatrixScreen::PixelColor::off);
}

void MatrixScreen::resetScreen(MatrixScreen::PixelColor color)
{
    std::fill(this->screen.begin(), this->screen.end(), color);
}


MatrixScreen::PixelColor MatrixScreen::operator()(std::size_t width, std::size_t height) const
{
    if (width >= this->pixelCountWidth) {
        std::stringstream ss;
        ss << "width out of range: max: " << this->pixelCountWidth << " got " << width;
        throw std::out_of_range(ss.str());
    }
    if (height >= this->pixelCountHeight) {
        std::stringstream ss;
        ss << "height out of range: max: " << this->pixelCountHeight << " got " << height;
        throw std::out_of_range(ss.str());
    }
    return this->screen.at(this->pixelCountWidth*height + width);
}


MatrixScreen::PixelColor& MatrixScreen::operator()(std::size_t width, std::size_t height)
{
    if (width >= this->pixelCountWidth) {
        std::stringstream ss;
        ss << "width out of range: max: " << this->pixelCountWidth << " got " << width;
        throw std::out_of_range(ss.str());
    }
    if (height >= this->pixelCountHeight) {
        std::stringstream ss;
        ss << "height out of range: max: " << this->pixelCountHeight << " got " << height;
        throw std::out_of_range(ss.str());
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
