#ifndef TRIANGLE2D_HPP
#define TRIANGLE2D_HPP

#include "point2d.hpp"
#include "matrix2d.hpp"


namespace Gfx2D {
    class Triangle{
        protected:
            Gfx2D::Point A, B, C;
        public:
            Triangle(const Gfx2D::Point& A, const Gfx2D::Point& B, const Gfx2D::Point& C);

            bool pointIsInTriangle(const Gfx2D::Point& p) const;

            Triangle createTransformedTriangle(const TransformationMatrix& mat) const;


    };
}


#endif //TRIANGLE2D_HPP
