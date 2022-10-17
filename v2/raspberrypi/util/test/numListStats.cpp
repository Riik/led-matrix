#include <gtest/gtest.h>

#include "numListStats.hpp"

TEST(NumListStats, sanity)
{
    NumListStats<long double> list(10);
    list.addNum(2);
    EXPECT_EQ(2, list.getMin());
    EXPECT_EQ(2, list.getMax());
    EXPECT_EQ(2, list.getAvg());
    EXPECT_EQ(1, list.getCount());
    list.reset();
    list.addNum(1);
    EXPECT_EQ(1, list.getMin());
    EXPECT_EQ(1, list.getMax());
    EXPECT_EQ(1, list.getAvg());
    EXPECT_EQ(1, list.getCount());
}

TEST(NumListStats, fullSine)
{
    NumListStats<long double> list(1000);

    for (std::size_t i = 0; i < 1000; ++i) {
        long double x = 2 * M_PI * (i/static_cast<long double>(999));
        list.addNum(std::sin(x));
    }
    EXPECT_EQ(list.getCount(), 1000);

    EXPECT_NEAR(list.getMax(), 1.0, 1e-4);
    EXPECT_NEAR(list.getMin(), -1.0, 1e-4);
    EXPECT_NEAR(list.getAvg(), 0.0, 1e-4);
    EXPECT_NEAR(list.getStandardDeviation(), 1/std::sqrt(2), 1e-3);

}
