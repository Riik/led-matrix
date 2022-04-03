#include <algorithm>

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
    return this->screen.at(this->pixelCountWidth*height + width);
}


MatrixScreen::PixelColor& MatrixScreen::operator()(std::size_t width, std::size_t height)
{
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
