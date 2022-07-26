#include <gtest/gtest.h>

#include "boundaryBox2d.hpp"

TEST(boundaryBox2d, overlapsWithSelf)
{
    Gfx2D::BoundaryBox box({1,1}, {0,0});
    EXPECT_TRUE(box.overlapsWith(box));
}

TEST(boundaryBox2d, nonOverlap)
{
    Gfx2D::BoundaryBox box({1,1}, {0,0});
    Gfx2D::BoundaryBox other({-1,1}, {-2,0});
    EXPECT_FALSE(box.overlapsWith(other));

    other = Gfx2D::BoundaryBox({3, 1}, {2, 0});
    EXPECT_FALSE(box.overlapsWith(other));

    other = Gfx2D::BoundaryBox({1, -1}, {0, -2});
    EXPECT_FALSE(box.overlapsWith(other));

    other = Gfx2D::BoundaryBox({1, 3}, {0, 2});
    EXPECT_FALSE(box.overlapsWith(other));
}

TEST(boundaryBox2d, partialOverlap)
{
    Gfx2D::BoundaryBox box({1,1}, {0,0});
    Gfx2D::BoundaryBox other({0.5,0.5}, {-0.5,-0.5});
    EXPECT_TRUE(box.overlapsWith(other));
}
