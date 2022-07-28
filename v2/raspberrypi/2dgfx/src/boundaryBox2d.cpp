#include "boundaryBox2d.hpp"

Gfx2D::BoundaryBox::BoundaryBox() {}

Gfx2D::BoundaryBox::BoundaryBox(Gfx2D::Point high, Gfx2D::Point low) : high(std::move(high)), low(std::move(low)) {}

bool Gfx2D::BoundaryBox::overlapsWith(const Gfx2D::BoundaryBox& other) const
{
    return (this->low.x() < other.high.x()) && (this->high.x() > other.low.x())
        && (this->high.y() > other.low.y()) && (this->low.y() < other.high.y());
}
