#include "solidColorTriangle2d.hpp"

Gfx2D::SolidColorTriangle::SolidColorTriangle(const Gfx2D::Point& A, const Gfx2D::Point& B, const Gfx2D::Point& C,
        const Gfx2D::CanvasDrawable::Color& color) : 
    Gfx2D::Triangle(A, B, C), color(color) {}

Gfx2D::SolidColorTriangle::SolidColorTriangle(const Gfx2D::Triangle& triangle, const Gfx2D::CanvasDrawable::Color& color) : 
    Gfx2D::Triangle(triangle), color(color) {}

bool Gfx2D::SolidColorTriangle::pointIsInDrawable(const Gfx2D::Point& point) const
{
   return this->pointIsInTriangle(point); 
}

Gfx2D::CanvasDrawable::Color Gfx2D::SolidColorTriangle::colorAtPoint(const Gfx2D::Point& /*unused*/) const
{
    return this->color;
}

Gfx2D::SolidColorTriangle Gfx2D::SolidColorTriangle::createTransformedTriangle(const Gfx2D::TransformationMatrix& mat) const
{
    Gfx2D::Triangle triangle = this->Gfx2D::Triangle::createTransformedTriangle(mat);
    return Gfx2D::SolidColorTriangle(triangle, this->color);
}
