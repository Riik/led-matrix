#include <gtest/gtest.h>

#include "vector3.hpp"

TEST(Vector3Test, constructor_equality)
{
    Vector3 v;
    Vector3 w(0,0,0);
    Vector3 u(v);

    EXPECT_EQ(v, w);
    EXPECT_EQ(v, u);
    EXPECT_EQ(w, u);
    EXPECT_EQ(w, v);
    EXPECT_EQ(u, v);
    EXPECT_EQ(u, w);
}

TEST(Vector3Test, scalar_multiplication)
{
    Vector3 v(1,1,1);
    v *= 2;
    EXPECT_EQ(v, Vector3(2,2,2));
    EXPECT_EQ(v*2, Vector3(4,4,4));
    EXPECT_EQ(2*v, Vector3(4,4,4));
    EXPECT_EQ(2*v*2, Vector3(8,8,8));
    EXPECT_EQ(v, Vector3(2,2,2));

    v = Vector3(1,2,3);
    EXPECT_EQ(v*2, Vector3(2,4,6));
    EXPECT_EQ(2*v, Vector3(2,4,6));
    EXPECT_EQ(2*v*2, Vector3(4,8,12));
    EXPECT_EQ(v, Vector3(1,2,3));
}

TEST(Vector3Test, vector_addition)
{
    Vector3 u(1,1,1);
    EXPECT_EQ(u+u, Vector3(2,2,2));

    Vector3 w(-1, -1, -1);
    u+=w;
    EXPECT_EQ(u, Vector3(0,0,0));
}

TEST(Vector3Test, vector_subtraction)
{
    Vector3 u(1,1,1);
    EXPECT_EQ(u-u, Vector3(0,0,0));

    Vector3 w(-1, -1, -1);
    u-=w;
    EXPECT_EQ(u, Vector3(2,2,2));
}

TEST(Vector3Test, array_operator)
{
    Vector3 v(0,0,0);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 0);
    EXPECT_EQ(v[2], 0);
    EXPECT_THROW(v[4], std::range_error);
}
