#pragma once

#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "types.h"

namespace silk
{

struct AddOrder;

class Parser
{
public:
    AddOrder ParseAdd(const std::uint8_t message[36]) const;

private:
};

template <std::integral T, size_t size = sizeof(T)> inline auto ParseValue(const uint8_t* pMessage)
{
    if constexpr (std::same_as<T, char> && size > 1)
    {
        std::array<char, size> buf{};
        std::memcpy(buf.data(), pMessage, size);
        return buf;
    }
    else
    {
        T value{};
        // In the case size < sizeof(T) we want to copy into the high bytes before
        // byteswapping
        // TODO: In the case where size is not a full width read (i.e. 6 bytes for
        // a timestamp) the memcpy compiles into multiple reads. Optimization would
        // be to overread, but this requires pMessage to be at least sizeof(T)
        // long.
        // Some compiler might already do this anyway
        std::memcpy(reinterpret_cast<uint8_t*>(&value) + (sizeof(T) - size), pMessage, size);
        if constexpr (std::endian::native == std::endian::little)
        {
            value = std::byteswap(value);
        }
        return value;
    }
}
}  // namespace silk
