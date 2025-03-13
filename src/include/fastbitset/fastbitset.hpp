#pragma once
#include <array>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace fastbitset
{

template<std::size_t CONTAINER_SIZE, typename CHUNK_TYPE = std::uint64_t>
class FastBitset
{
    static_assert(std::is_integral_v<CHUNK_TYPE>, "CHUNK_TYPE must be an integral type");
    static_assert(std::is_unsigned_v<CHUNK_TYPE>, "CHUNK_TYPE must be an unsigned integer");
    static constexpr auto bitsPerChunk = std::numeric_limits<CHUNK_TYPE>::digits;
    static constexpr auto numberOfChunks = (CONTAINER_SIZE + bitsPerChunk - 1) / bitsPerChunk;


public:
    FastBitset()
    {
        clear();
    }

    template<typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
    FastBitset(T initialValue)
    {
        clear();
        if constexpr (canTypeStoreMoreBitsThanContainer<T>())
        {
            auto truncatedValue = truncateToContainerSize(initialValue);
            storeValue(truncatedValue);
        }
        else
        {
            storeValue(initialValue);
        }
    }

    [[nodiscard]] constexpr int size() const
    {
        return CONTAINER_SIZE;
    }

    void set()
    {
        mData.fill(std::numeric_limits<CHUNK_TYPE>::max());
        truncateContainerToMaxSize();
    }

    void clear()
    {
        mData.fill(0);
    }

    CHUNK_TYPE chunk(std::size_t index)
    {
        return mData.at(index);
    }


private:
    template<typename T>
    static constexpr bool canTypeStoreMoreBitsThanContainer()
    {
        return std::numeric_limits<T>::digits > CONTAINER_SIZE;
    }

    template<typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
    constexpr void storeValue(T value)
    {
        for (std::size_t i = 0; i < numberOfChunks and value != 0; ++i)
        {
            mData[i] = static_cast<CHUNK_TYPE>(value);
            value >>= bitsPerChunk;
        }
    }

    template<typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
    constexpr T truncateToContainerSize(T valueToTruncate) const
    {
        valueToTruncate &= (T{1} << CONTAINER_SIZE) - 1;
        return valueToTruncate;
    }

    constexpr void truncateContainerToMaxSize()
    {
        constexpr auto remainderBits = CONTAINER_SIZE % bitsPerChunk;

        if constexpr (remainderBits != 0)
        {
            constexpr auto mask = (CHUNK_TYPE{1} << remainderBits) - 1;
            mData.back() &= mask;
        }
    }

private:
    std::array<CHUNK_TYPE, numberOfChunks> mData;
};

}// namespace fastbitset