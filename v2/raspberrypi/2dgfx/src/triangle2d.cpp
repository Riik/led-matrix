#include "triangle2d.hpp"

Gfx2D::Triangle::Triangle(const Gfx2D::Point& A, const Gfx2D::Point& B, const Gfx2D::Point& C) : A{A}, B{B}, C{C} {}

static float sign(const Gfx2D::Point& p1, const Gfx2D::Point& p2, const Gfx2D::Point& p3)
{
    return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
}

bool Gfx2D::Triangle::pointIsInTriangle(const Gfx2D::Point& p) const
{
    float d1 = sign(p, this->A, this->B);
    float d2 = sign(p, this->B, this->C);
    float d3 = sign(p, this->C, this->A);

    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);

    return !(has_neg && has_pos);
}

Gfx2D::Triangle Gfx2D::Triangle::createTransformedTriangle(const Gfx2D::TransformationMatrix& mat) const
{
    return Gfx2D::Triangle(mat*this->A, mat*this->B, mat*this->C);
}
