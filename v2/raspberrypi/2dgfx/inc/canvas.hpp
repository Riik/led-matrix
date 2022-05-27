#ifndef CANVAS_HPP
#define CANVAS_HPP
#include <vector>
#include <functional>

#include "matrixScreen.hpp"
#include "canvasDrawable.hpp"

namespace Gfx2D {
    class Canvas {
        public:
            Canvas(const MatrixScreen& referenceScreen, const CanvasDrawable::Color& backgroundColor);

            // Add a new drawable. This also introduces a virtual z coordinate: a drawable added later is considered
            // on top of those added earlier, when more than one drawable covers the same spot.
            void addToFrame(const CanvasDrawable& drawable);

            // Draw all drawables to a MatrixScreen and then clear the list of drawables.
            const MatrixScreen generateFrame();
        private:

            CanvasDrawable::Color getColorOfPixel(const Gfx2D::Point& pixelIndex) const;

            const MatrixScreen referenceScreen;
            const CanvasDrawable::Color backgroundColor;
            std::vector<std::reference_wrapper<const CanvasDrawable>> drawables;
    };
}

#endif //CANVAS_HPP
