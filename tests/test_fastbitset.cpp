#include "fastbitset/fastbitset.hpp"
#include <gtest/gtest.h>

TEST(FastBitsetTest, DefaultConstructor)
{
    fastbitset::FastBitset fb;
    EXPECT_EQ(fb.size(), 0);
}