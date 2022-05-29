#include <stdexcept>
#include <array>

#include "matrixScreen.hpp"
#include "canvas.hpp"
#include "point2d.hpp"

Gfx2D::Canvas::Canvas(const MatrixScreen& referenceScreen, const Gfx2D::CanvasDrawable::Color& backgroundColor) :
    referenceScreen(referenceScreen), backgroundColor(backgroundColor)
{
    if (backgroundColor == Gfx2D::CanvasDrawable::Color::transparent) {
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
}

void Gfx2D::Canvas::addToFrame(const Gfx2D::CanvasDrawable& drawable)
{
    this->drawables.push_back(drawable);
}

Gfx2D::CanvasDrawable::Color Gfx2D::Canvas::getColorOfPixel(const Gfx2D::Point& pixelIndex) const
{
    static const std::vector<Gfx2D::Point> coordinates = {
        Gfx2D::Point(0.5, 0.0),
        Gfx2D::Point(0.0, 0.5),
        Gfx2D::Point(0.5, 0.5),
        Gfx2D::Point(1.0, 0.5),
        Gfx2D::Point(0.5, 1.0)
    };
    unsigned int onCount = 0;
    unsigned int offCount = 0;

    for (auto c : coordinates) {
        Gfx2D::Point testPoint = this->pixelTransformation * (c + pixelIndex);
        Gfx2D::CanvasDrawable::Color color = Gfx2D::CanvasDrawable::Color::transparent;
        for (auto rit = drawables.crbegin(); rit != drawables.crend(); ++rit) {
            if (rit->get().pointIsInDrawable(testPoint)) {
                color = rit->get().colorAtPoint(testPoint);
            }
            if (color != Gfx2D::CanvasDrawable::Color::transparent) {
                break;
            }
        }
        if (color == Gfx2D::CanvasDrawable::Color::transparent) {
            color = this->backgroundColor;
        }

        if (color == Gfx2D::CanvasDrawable::Color::on) {
            onCount++;
        } else {
            offCount++;
        }
    }

    if (onCount > offCount) {
        return Gfx2D::CanvasDrawable::Color::on;
    } else {
        return Gfx2D::CanvasDrawable::Color::off;
    }
}

const MatrixScreen Gfx2D::Canvas::generateFrame()
{
    MatrixScreen output(this->referenceScreen);

    for (size_t x = 0; x < output.getPixelCountWidth(); ++x) {
        for (size_t y = 0; y < output.getPixelCountHeight(); ++y) {
            if (this->getColorOfPixel(Gfx2D::Point(x, y)) == Gfx2D::CanvasDrawable::Color::on) {
                output(x,y) = MatrixScreen::PixelColor::on;
            } else {
                output(x,y) = MatrixScreen::PixelColor::off;
            }
        }
    }

    this->drawables.clear();
    return output;
}
