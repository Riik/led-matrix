#ifndef POINT2D_HPP
#define POINT2D_HPP

#include "matrix.hpp"

namespace Gfx2D {
    // 3 rows because of homogenous coordinate system.
    class Point: public Matrix<3, 1>
    {
        public:
            Point(float x, float y) : Matrix<3, 1>({x, y, 1}) {}
            Point(const Matrix<3,1>& mat) : Matrix<3, 1>(mat) {}
    };
}
#endif //POINT2D_HPP
