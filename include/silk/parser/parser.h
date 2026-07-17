#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <cstring>

namespace silk
{

struct AddOrder;

class Parser
{
public:
    AddOrder ParseAdd(const std::uint8_t message[36]) const;

private:
};

template <std::integral T> inline T ParseValue(const uint8_t* pMessage)
{
    T value;
    std::memcpy(&value, pMessage, sizeof(T));
    if constexpr (std::endian::native == std::endian::little)
    {
        value = std::byteswap(value);
    }
    return value;
}
}  // namespace silk
