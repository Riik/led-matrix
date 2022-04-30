#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "point2d.hpp"
#include "matrix2d.hpp"

MATCHER_P(PointEq, exp,  "") {
    const float delta = 1e-5;
    for (std::size_t i = 0; i < 3; ++i) {
        if(std::abs(arg(i, 0) - exp(i, 0)) > delta) {
            return false;
        }
    }
    return true;
}

TEST(PointMatrix2D, constructor)
{
    Gfx2D::Point a = Gfx2D::Point(1, 2);
    Matrix<3, 1> b = {1, 2, 1};
    EXPECT_EQ(a, b);

    Matrix<3,3> c = {1, 0, 0,
                     0, 1, 0,
                     0, 0, 1};
    EXPECT_EQ(Gfx2D::createIdentityMatrix(), c);
}

TEST(PointMatrix2D, translation)
{
    Gfx2D::Point a(-1, 1);
    Gfx2D::TransformationMatrix translate = Gfx2D::createTranslationMatrix(2, -2);

    Gfx2D::Point out = translate*a;
    Gfx2D::Point exp(1, -1);

    EXPECT_THAT(out, PointEq(exp));
}

TEST(PointMatrix2D, rotation)
{
    Gfx2D::Point a(-1, 1);
    Gfx2D::Point exp(1, -1);
    Gfx2D::TransformationMatrix rotate = Gfx2D::createRotationMatrix(M_PI);
    Gfx2D::Point out = rotate * a;

    EXPECT_THAT(out, PointEq(exp));
}

TEST(PointMatrix2D, scaling)
{
    Gfx2D::Point a(-1, 1);
    Gfx2D::Point exp(-2, 4);
    Gfx2D::TransformationMatrix scale = Gfx2D::createScaleMatrix(2, 4);
    Gfx2D::Point out = scale * a;

    EXPECT_THAT(out, PointEq(exp));
}
