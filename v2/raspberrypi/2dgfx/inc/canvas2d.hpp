#pragma once

#include <vector>
#include <functional>

#include "matrixScreen.hpp"
#include "canvas2dDrawable.hpp"
#include "matrix2d.hpp"

namespace Gfx2D {
    // For the canvas, the center of the screen is at 0,0. The short side (which is both sides if the screen is a square) goes from -1.0 to 1.0.
    // The long side (if any) follows logically: If the screen is twice as wide as it is high, then the height goes from -1.0 to 1.0 and
    // the width goes from -2.0 to 2.0. The size of the canvas is called the clipping space, everything outside of it will not be
    // rendered.
    class Canvas {
        public:
            Canvas(const MatrixScreen& referenceScreen, const PixelColor& backgroundColor);

            // Add a new drawable. This also introduces a virtual z coordinate: a drawable added later is considered
            // on top of those added earlier, when more than one drawable covers the same spot.
            void addToFrame(const CanvasDrawable& drawable);

            // Draw all drawables to a MatrixScreen and then clear the list of drawables.
            MatrixScreen generateFrame();
        private:

            PixelColor getColorOfPixel(const Gfx2D::Point& pixelIndex) const;
            PixelColor getColorOfPoint(const Gfx2D::Point& point) const;

            MatrixScreen referenceScreen;
            PixelColor backgroundColor;
            std::vector<std::reference_wrapper<const CanvasDrawable>> drawables;

            Gfx2D::Point bottomLeftCoordinate;
            float totalHeight;
            float totalWidth;
            Gfx2D::TransformationMatrix pixelTransformation;

            BoundaryBox boundaryBox;
    };
}
