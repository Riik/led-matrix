#pragma once

#include "point2d.hpp"

namespace Gfx2D {
    class BoundaryBox {
        private:
            // high.x should contain the biggest (rightmost) x coordinate, high.y should contain the biggest (topmost) y coordinate
            Point high;
            Point low;
        public:
            BoundaryBox();
            BoundaryBox(Point high, Point low);
            bool overlapsWith(const BoundaryBox& other) const;
    };
}
