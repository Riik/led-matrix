#pragma once

#include <functional>

#include "canvas2dDrawable.hpp"
#include "triangle2d.hpp"
#include "texture2d.hpp"

namespace Gfx2D {
    class TexturedTriangle : public CanvasDrawable, public Triangle {
        public:
            struct TextureMapping {
                Point A;
                Point B;
                Point C;
            };

            TexturedTriangle(Point A, Point B, Point C, const Texture& texture, TextureMapping mapping);
            TexturedTriangle(Triangle triangle, const Texture& texture, TextureMapping mapping);

            virtual bool pointIsInDrawable(const Point& p) const override final;
            virtual PixelColor colorAtPoint(const Point& p) const override final;

            TexturedTriangle createTransformedTriangle(const TransformationMatrix& mat) const;
            TexturedTriangle createTransformedTriangle(const TransformationMatrix& mat, const Texture& newTexture) const;
        private:
            std::reference_wrapper<const Texture> texture;
            TextureMapping mapping;
    };
}
