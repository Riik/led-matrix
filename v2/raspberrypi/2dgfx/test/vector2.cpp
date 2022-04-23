#include <gtest/gtest.h>

#include "vector2.hpp"

TEST(Vector2Test, simple_constructor)
{
    Vector2 v;
    EXPECT_EQ(v.x(), 0);
    EXPECT_EQ(v.y(), 0);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 0);
}

TEST(Vector2Test, constructor_equality)
{
    Vector2 v;
    Vector2 w(0,0);
    Vector2 u(v);

    EXPECT_EQ(v, w);
    EXPECT_EQ(v, u);
    EXPECT_EQ(w, u);
    EXPECT_EQ(w, v);
    EXPECT_EQ(u, v);
    EXPECT_EQ(u, w);
}

TEST(Vector2Test, scalar_multiplication)
{
    Vector2 v(1,1);
    v *= 2;
    EXPECT_EQ(v, Vector2(2,2));
    EXPECT_EQ(v*2, Vector2(4,4));
    EXPECT_EQ(2*v, Vector2(4,4));
    EXPECT_EQ(2*v*2, Vector2(8,8));
    EXPECT_EQ(v, Vector2(2,2));

    v = Vector2(1,2);
    EXPECT_EQ(v*2, Vector2(2,4));
    EXPECT_EQ(2*v, Vector2(2,4));
    EXPECT_EQ(2*v*2, Vector2(4,8));
    EXPECT_EQ(v, Vector2(1,2));
}
