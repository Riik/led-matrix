#pragma once

#include "point2d.hpp"
#include "pixelColor.hpp"
#include "boundaryBox2d.hpp"

namespace Gfx2D {

    class CanvasDrawable {
        public:
            virtual bool pointIsInDrawable(const Point& p) const = 0;
            // This function only returns a valid color if p is in the drawable.
            // If the point p is not in the drawable, this function can return whatever it wants.
            virtual PixelColor colorAtPoint(const Point& p) const = 0;
            // This function guarantees to return true if there is overlap
            // between the boundary box and the drawable
            virtual bool drawableOverlapsWith(const BoundaryBox& other) const = 0;
    };
}
