#ifndef MATRIX2D_HPP
#define MATRIX2D_HPP
#include <cmath>

#include "matrix.hpp"

namespace Gfx2D {

    using TransformationMatrix = Matrix<3,3>;

    inline TransformationMatrix createIdentityMatrix()
    {
        return TransformationMatrix({1, 0, 0,
                0, 1, 0,
                0, 0, 1});
    }

    inline TransformationMatrix createRotationMatrix(float phi)
    {
        return TransformationMatrix({std::cos(phi), -std::sin(phi), 0,
                std::sin(phi),  std::cos(phi), 0,
                0,              0,             1});
    }

    inline TransformationMatrix createTranslationMatrix(float xTrans, float yTrans)
    {
        return TransformationMatrix({1, 0, xTrans,
                0, 1, yTrans,
                0, 0, 1});
    }

    inline TransformationMatrix createScaleMatrix(float xScale, float yScale)
    {
        return TransformationMatrix({xScale, 0, 0,
                0, yScale, 0,
                0, 0,      1});
    }
}
#endif //MATRIX2D_HPP
