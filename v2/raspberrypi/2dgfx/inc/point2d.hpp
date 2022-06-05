#ifndef POINT2D_HPP
#define POINT2D_HPP

#include "matrix.hpp"

namespace Gfx2D {
    // 3 rows because of homogenous coordinate system.
    class Point: public Matrix<3, 1>
    {
        public:
            // Note that this constructor allows implicit conversion from {<float>,<float>} to Point.
            Point(float x, float y) : Matrix<3, 1>({x, y, 1}) {}
            Point(const Matrix<3,1>& mat) : Matrix<3, 1>({mat(0, 0), mat(1,0), 1}) {}
            Point() : Matrix<3,1>() {}

            float x() const
            {
                return this->arr[0];
            }

            float y() const
            {
                return this->arr[1];
            }

            Point& operator+=(const Point& other)
            {
                for (std::size_t i = 0; i < 2; ++i) {
                    this->arr[i] += other.arr[i];
                }
                this->arr[2] = 1.0f;
                return *this;
            }
    };

    inline Point operator+(Point lhs, const Point& rhs)
    {
        return lhs += rhs;
    }
}
#endif //POINT2D_HPP
