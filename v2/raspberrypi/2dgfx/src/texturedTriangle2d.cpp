#include "texturedTriangle2d.hpp"

Gfx2D::TexturedTriangle::TexturedTriangle(Gfx2D::Point A, Gfx2D::Point B, Gfx2D::Point C, const Gfx2D::Texture& texture,
        Gfx2D::TexturedTriangle::TextureMapping mapping) :
    Gfx2D::Triangle(A, B, C), texture(texture), mapping(mapping) {}

Gfx2D::TexturedTriangle::TexturedTriangle(Gfx2D::Triangle triangle, const Gfx2D::Texture& texture,
        Gfx2D::TexturedTriangle::TextureMapping mapping) :
    Gfx2D::Triangle(triangle), texture(texture), mapping(mapping) {}

bool Gfx2D::TexturedTriangle::pointIsInDrawable(const Gfx2D::Point& point) const
{
   return this->pointIsInTriangle(point);
}

PixelColor Gfx2D::TexturedTriangle::colorAtPoint(const Gfx2D::Point& point) const
{
    Gfx2D::Point p0 = this->B - this->A;
    Gfx2D::Point p1 = this->C - this->A;
    Gfx2D::Point p2 = point - this->A;
    float den = p0.x() * p1.y() - p1.x() * p0.y();
    float v = (p2.x() * p1.y() - p1.x() * p2.y()) / den;
    float w = (p0.x() * p2.y() - p2.x() * p0.y()) / den;
    float u = 1.0f - v - w;

    Gfx2D::Point texelCoords = this->mapping.A * u + this->mapping.B * v + this->mapping.C * w;

    return this->texture.get().getColorAt(texelCoords.x(), texelCoords.y());
}

Gfx2D::TexturedTriangle Gfx2D::TexturedTriangle::createTransformedTriangle(const Gfx2D::TransformationMatrix& mat,
        const Gfx2D::Texture& newTexture) const
{
    Gfx2D::Triangle triangle = this->Gfx2D::Triangle::createTransformedTriangle(mat);
    return Gfx2D::TexturedTriangle(triangle, newTexture, this->mapping);
}


Gfx2D::TexturedTriangle Gfx2D::TexturedTriangle::createTransformedTriangle(const Gfx2D::TransformationMatrix& mat) const
{
    return this->createTransformedTriangle(mat, this->texture);
}
