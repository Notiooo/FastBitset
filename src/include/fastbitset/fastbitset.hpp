#pragma once
#include <array>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace fastbitset
{

class BitsetOverflow : public std::runtime_error
{
public:
    explicit BitsetOverflow(const char* msg)
        : std::runtime_error(msg)
    {
    }
};

template<std::size_t CONTAINER_SIZE, typename CHUNK_TYPE = std::uint64_t>
class FastBitset
{
    static_assert(CONTAINER_SIZE != 0, "Container can't be of size 0.");
    static_assert(std::is_unsigned_v<CHUNK_TYPE>, "CHUNK_TYPE must be an unsigned integer");
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

        if (not fitsInContainer(initialValue))
        {
            throw BitsetOverflow("FastBitset: value out of range. Capacity of container can't "
                                 "store whole provided value to the container.");
        }
        storeValue(initialValue);
    }

    [[nodiscard]] constexpr int size() const
    {
        return CONTAINER_SIZE;
    }

    void set()
    {
        mData.fill(std::numeric_limits<CHUNK_TYPE>::max());
        truncateExcessBits();
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
    static constexpr bool canContainerStoreWholeTypeSize()
    {
        return CONTAINER_SIZE >= std::numeric_limits<T>::digits;
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

    constexpr void truncateExcessBits()
    {
        constexpr auto remainderBits = CONTAINER_SIZE % bitsPerChunk;

        if constexpr (remainderBits != 0)
        {
            constexpr auto mask = (CHUNK_TYPE{1} << remainderBits) - 1;
            mData.back() &= mask;
        }
    }

    template<typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
    static constexpr bool fitsInContainer(T value) noexcept
    {
        if (canContainerStoreWholeTypeSize<T>())
        {
            return true;
        }
        return value < (T{1} << CONTAINER_SIZE);
    }

private:
    std::array<CHUNK_TYPE, numberOfChunks> mData;
};

}// namespace fastbitset