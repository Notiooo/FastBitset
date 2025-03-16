#include "fastbitset/fastbitset.hpp"
#include <gtest/gtest.h>

using FastBitset16_U08 = fastbitset::FastBitset<16, std::uint8_t>;

TEST(FastBitset16_U08, SizeShouldMatchGivenTemplateParameters)
{
    FastBitset16_U08 bitset;
    EXPECT_EQ(bitset.size(), 16);
}

TEST(FastBitset16_U08, DefaultConstructorShouldInitializeToZero)
{
    FastBitset16_U08 bitset;
    EXPECT_EQ(bitset.chunk(0), 0b0000'0000);
    EXPECT_EQ(bitset.chunk(1), 0b0000'0000);
}

TEST(FastBitset16_U08, ConstructorShouldAllowToSetValueWithinOneChunk)
{
    const auto valueWithinOneChunk = std::uint8_t{0b1010'1010};
    FastBitset16_U08 bitset(valueWithinOneChunk);
    EXPECT_EQ(bitset.chunk(0), 0b1010'1010);
    EXPECT_EQ(bitset.chunk(1), 0b0000'0000);
}

TEST(FastBitset16_U08, ConstructorShouldAllowToSetValueAcrossTwoChunks)
{
    const auto valueAccrossTwoChunks = std::uint16_t{0b1110'0001'1010'1010};
    FastBitset16_U08 bitset(valueAccrossTwoChunks);
    EXPECT_EQ(bitset.chunk(0), 0b1010'1010);
    EXPECT_EQ(bitset.chunk(1), 0b1110'0001);
}

TEST(FastBitset16_U08, ConstructorShouldThrowWhenValueBeyondCapacity)
{
    const auto beyondValue = std::uint32_t{0b1111'1111'1111'1111'1110'0001'1010'1010};

    EXPECT_THROW(FastBitset16_U08 bitset(beyondValue), fastbitset::BitsetOverflow);
}

TEST(FastBitset16_U08, ClearShouldSetAllBitsToZero)
{
    FastBitset16_U08 bitset;
    bitset.set();
    EXPECT_EQ(bitset.chunk(0), 0b1111'1111);
    EXPECT_EQ(bitset.chunk(1), 0b1111'1111);
    bitset.clear();
    EXPECT_EQ(bitset.chunk(0), 0b0000'0000);
    EXPECT_EQ(bitset.chunk(1), 0b0000'0000);
}

TEST(FastBitset16_U08, SetShouldSetAllBitsToOne)
{
    FastBitset16_U08 bitset;
    EXPECT_EQ(bitset.chunk(0), 0b0000'0000);
    EXPECT_EQ(bitset.chunk(1), 0b0000'0000);
    bitset.set();
    EXPECT_EQ(bitset.chunk(0), 0b1111'1111);
    EXPECT_EQ(bitset.chunk(1), 0b1111'1111);
}
