#include <gtest/gtest.h>

#include "triangle2d.hpp"

TEST(Triangle2D, pointInTriangle)
{
    Gfx2D::Triangle tr(Gfx2D::Point(-1, 0), Gfx2D::Point(1, 0), Gfx2D::Point(0, 1));

    EXPECT_EQ(true, tr.pointIsInTriangle(Gfx2D::Point(0, 0.5)));
    EXPECT_EQ(false, tr.pointIsInTriangle(Gfx2D::Point(0, -0.5)));
}

TEST(Triangle2D, transformedTriangle)
{

    Gfx2D::Triangle tr(Gfx2D::Point(-1, 0), Gfx2D::Point(1, 0), Gfx2D::Point(0, 1));
    Gfx2D::TransformationMatrix move = Gfx2D::createTranslationMatrix(0, -1);
    Gfx2D::Triangle trTranslated = tr.createTransformedTriangle(move);

    EXPECT_EQ(false, trTranslated.pointIsInTriangle(Gfx2D::Point(0, 0.5)));
    EXPECT_EQ(true, trTranslated.pointIsInTriangle(Gfx2D::Point(0, -0.5)));
}
