#pragma once
#include <array>
#include <concepts>
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

template<std::size_t CONTAINER_SIZE, std::unsigned_integral CHUNK_TYPE = std::uint64_t>
class FastBitset
{
    static_assert(CONTAINER_SIZE != 0, "Container can't be of size 0.");
    static_assert(std::is_unsigned_v<CHUNK_TYPE>, "CHUNK_TYPE must be an unsigned integer");
    static_assert(std::is_unsigned_v<CHUNK_TYPE>, "CHUNK_TYPE must be an unsigned integer");
    static constexpr auto BITS_PER_CHUNK = std::numeric_limits<CHUNK_TYPE>::digits;
    static constexpr auto NUMBER_OF_CHUNKS = (CONTAINER_SIZE + BITS_PER_CHUNK - 1) / BITS_PER_CHUNK;


public:
    constexpr FastBitset() noexcept = default;

    template<std::unsigned_integral T>
    explicit FastBitset(T initialValue)
    {
        if (not fitsInContainer(initialValue))
        {
            throw BitsetOverflow("FastBitset: value out of range. Capacity of container can't "
                                 "store whole provided value to the container.");
        }
        storeValue(initialValue);
    }

    [[nodiscard]] static constexpr std::size_t size() noexcept
    {
        return CONTAINER_SIZE;
    }

    void set() noexcept
    {
        mData.fill(std::numeric_limits<CHUNK_TYPE>::max());
        truncateExcessBits();
    }

    void clear() noexcept
    {
        mData.fill(CHUNK_TYPE{0});
    }

    [[nodiscard]] CHUNK_TYPE chunk(std::size_t index) const
    {
        return mData.at(index);
    }


private:
    template<std::unsigned_integral T>
    static constexpr bool canContainerStoreWholeTypeSize()
    {
        return CONTAINER_SIZE >= std::numeric_limits<T>::digits;
    }

    template<std::unsigned_integral T>
    constexpr void storeValue(T value)
    {
        for (std::size_t i = 0; i < NUMBER_OF_CHUNKS and value != 0; ++i)
        {
            mData[i] = static_cast<CHUNK_TYPE>(value);
            value >>= BITS_PER_CHUNK;
        }
    }

    constexpr void truncateExcessBits() noexcept
    {
        constexpr auto remainderBits = CONTAINER_SIZE % BITS_PER_CHUNK;

        if constexpr (remainderBits != 0)
        {
            constexpr auto mask = (CHUNK_TYPE{1} << remainderBits) - 1;
            mData.back() &= mask;
        }
    }

    template<std::unsigned_integral T>
    static constexpr bool fitsInContainer(T value) noexcept
    {
        if (canContainerStoreWholeTypeSize<T>())
        {
            return true;
        }
        return value < (T{1} << CONTAINER_SIZE);
    }

private:
    std::array<CHUNK_TYPE, NUMBER_OF_CHUNKS> mData{};
};

}// namespace fastbitset