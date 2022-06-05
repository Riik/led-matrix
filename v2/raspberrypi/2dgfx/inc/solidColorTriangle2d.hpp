#pragma once

#include "canvas2dDrawable.hpp"
#include "triangle2d.hpp"

namespace Gfx2D {
    class SolidColorTriangle : public CanvasDrawable, public Triangle {
        private:
            PixelColor color;
        public:
            SolidColorTriangle(const Point& A, const Point& B, const Point& C, const PixelColor& color);
            SolidColorTriangle(const Triangle& triangle, const PixelColor& color);

            virtual bool pointIsInDrawable(const Point& p) const override final;
            virtual PixelColor colorAtPoint(const Point& p) const override final;

            SolidColorTriangle createTransformedTriangle(const TransformationMatrix& mat) const;
    };
}
