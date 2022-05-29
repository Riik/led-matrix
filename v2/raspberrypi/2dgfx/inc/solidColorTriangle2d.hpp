#pragma once

#include "canvasDrawable.hpp"
#include "triangle2d.hpp"

namespace Gfx2D {
    class SolidColorTriangle : public CanvasDrawable, public Triangle {
        private:
            CanvasDrawable::Color color;
        public:
            SolidColorTriangle(const Point& A, const Point& B, const Point& C, const CanvasDrawable::Color& color);
            SolidColorTriangle(const Triangle& triangle, const CanvasDrawable::Color& color);

            virtual bool pointIsInDrawable(const Point& p) const override final;
            virtual Color colorAtPoint(const Point& p) const override final;

            SolidColorTriangle createTransformedTriangle(const TransformationMatrix& mat) const;
    };
}
