#include "fastbitset/fastbitset.hpp"
#include <gtest/gtest.h>

using FastBitset08_U08 = fastbitset::FastBitset<8, std::uint8_t>;

TEST(FastBitset08_U08, SizeShouldMatchGivenTemplateParameters)
{
    FastBitset08_U08 bitset;
    EXPECT_EQ(bitset.size(), 8);
}

TEST(FastBitset08_U08, DefaultConstructorShouldInitializeToZero)
{
    FastBitset08_U08 bitset;
    EXPECT_EQ(bitset.chunk(0), 0b0000'0000);
}

TEST(FastBitset08_U08, ConstructorShouldAllowToInitializeInRangeValue)
{
    const auto properValue = std::uint8_t{0b1010'1010};
    FastBitset08_U08 bitset(properValue);
    EXPECT_EQ(bitset.chunk(0), 0b1010'1010);
}

TEST(FastBitset08_U08, ConstructorShouldTruncateOutOfRangeValues)
{
    const auto biggerValueThanContainer = std::uint16_t{0b1111'1111'1111'0000};
    FastBitset08_U08 bitset(biggerValueThanContainer);
    EXPECT_EQ(bitset.chunk(0), 0b1111'0000);
}

TEST(FastBitset08_U08, ClearShouldSetAllBitsToZero)
{
    FastBitset08_U08 bitset;
    bitset.set();
    EXPECT_EQ(bitset.chunk(0), 0b1111'1111);
    bitset.clear();
    EXPECT_EQ(bitset.chunk(0), 0b0000'0000);
}

TEST(FastBitset08_U08, SetShouldSetAllBitsToOne)
{
    FastBitset08_U08 bitset;
    EXPECT_EQ(bitset.chunk(0), 0b0000'0000);
    bitset.set();
    EXPECT_EQ(bitset.chunk(0), 0b1111'1111);
}
