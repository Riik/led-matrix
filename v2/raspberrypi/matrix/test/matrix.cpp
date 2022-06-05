#include <gtest/gtest.h>

#include "matrix.hpp"

TEST(templatedMatrixTest, constructors)
{
    Matrix<2,2> mat;
    Matrix<2,2> mat2({0,0,0,0});
    EXPECT_EQ(mat, mat2);
    Matrix<2,2> mat3 = {1,0,1,0};
    EXPECT_NE(mat3, mat);
}

TEST(templatedMatrixTest, floatMultiplication)
{
    Matrix<2,2> mat({1,1,1,1});
    Matrix<2,2> exp({3,3,3,3});
    Matrix<2,2> mat3 = mat * 3;

    EXPECT_EQ(mat3, exp);

    exp = Matrix<2,2>({1,1,1,1});
    EXPECT_EQ(mat, exp);

    exp = Matrix<2,2>({0.5, 0.5, 0.5, 0.5});
    mat *= 0.5;
    EXPECT_EQ(exp, mat);
}

TEST(templatedMatrixTest, floatDivision)
{
    Matrix<2,2> mat({1,1,1,1});
    Matrix<2,2> exp({0.5, 0.5, 0.5, 0.5});

    EXPECT_EQ(mat/2, exp);
    exp = {2,2,2,2};
    mat /=0.5;
    EXPECT_EQ(mat, exp);
}

TEST(templatedMatrixTest, matrixMultiplication)
{
    {
        // Scope this so we can recycle the variables
        Matrix<3, 2> a = {2,2,2,2,2,2};
        Matrix<2, 3> b = {3,3,3,3,3,3};
        Matrix<3, 3> c = {12, 12, 12,
                        12, 12, 12,
                        12, 12, 12};
        auto d = a*b;
        EXPECT_EQ(d, c);

        a = {2, 3,
            4, 5,
            4, 3};

        b = {3, 4, 5,
            4, 3, 2};

        c = {18, 17, 16,
            32, 31, 30,
            24, 25, 26};

        d = a*b;
        EXPECT_EQ(d, c);
    }
    {
        Matrix<4, 2> a = {2, 3,
                          3, 2,
                          2, 3,
                          4, 2};
        Matrix<2, 1> b = {3,
                          4};

        Matrix<4, 1> c = {18,
                          17,
                          18,
                          20};

        auto d = a*b;
        EXPECT_EQ(d, c);
    }
}
