#include <stdexcept>
#include <array>

#include "matrixScreen.hpp"
#include "canvas2d.hpp"
#include "point2d.hpp"

Gfx2D::Canvas::Canvas(const MatrixScreen& referenceScreen, const PixelColor& backgroundColor) :
    referenceScreen(referenceScreen), backgroundColor(backgroundColor)
{
    if (backgroundColor == PixelColor::transparent) {
        throw std::invalid_argument("The background of a canvas cannot be transparent");
    }

    // We now create a matrix which can remap a pixel coordinate to a coordinate in clipping space.
    // First step is to translate: The bottom left corner cannot be (0,0), it must be the bottom
    // left corner of the clipping space. Therefore, recenter.
    float xTrans = (float)(this->referenceScreen.getPixelCountWidth())/2.0f;
    float yTrans = (float)(this->referenceScreen.getPixelCountHeight())/2.0f;
    Gfx2D::TransformationMatrix translateMat = Gfx2D::createTranslationMatrix(-xTrans, -yTrans);

    // Now we scale down. Both directions are scaled the same amount to preserve aspect ratio. The shortest side must map to [-1.0, 1.0].
    float scale;
    if (xTrans < yTrans) {
        scale = 1.0f/xTrans;
    } else {
        scale = 1.0f/yTrans;
    }
    Gfx2D::TransformationMatrix scaleMat = Gfx2D::createScaleMatrix(scale, scale);

    // First translate, then scale.
    this->pixelTransformation = scaleMat*translateMat;

    // Determine the boundaryBox
    Gfx2D::Point min{0, 0};
    min = this->pixelTransformation * min;
    Gfx2D::Point max(this->referenceScreen.getPixelCountWidth(), this->referenceScreen.getPixelCountHeight());
    max = this->pixelTransformation * max;

    this->boundaryBox = Gfx2D::BoundaryBox(max, min);
}

void Gfx2D::Canvas::addToFrame(const Gfx2D::CanvasDrawable& drawable)
{
    if (drawable.drawableOverlapsWith(this->boundaryBox)) {
        this->drawables.push_back(drawable);
    }
}

PixelColor Gfx2D::Canvas::getColorOfPoint(const Gfx2D::Point& point) const
{
    PixelColor color = PixelColor::transparent;
    for (auto rit = this->drawables.crbegin(); rit != drawables.crend(); ++rit) {
        if (rit->get().pointIsInDrawable(point)) {
            color = rit->get().colorAtPoint(point);
        }
        if (color != PixelColor::transparent) {
            break;
        }
    }
    if (color == PixelColor::transparent) {
        color = this->backgroundColor;
    }
    return color;

}

PixelColor Gfx2D::Canvas::getColorOfPixel(const Gfx2D::Point& pixelIndex) const
{
    Gfx2D::Point pointOffset(0.5, 0.5);
    Gfx2D::Point testPoint = this->pixelTransformation * (pixelIndex + pointOffset);
    return this->getColorOfPoint(testPoint);
}

MatrixScreen Gfx2D::Canvas::generateFrame()
{
    MatrixScreen output(this->referenceScreen);

    for (size_t y = 0; y < output.getPixelCountHeight(); ++y) {
        for (size_t x = 0; x < output.getPixelCountWidth(); ++x) {
            output(x, y) = this->getColorOfPixel(Gfx2D::Point(x, y));
        }
    }

    this->drawables.clear();
    return output;
}
