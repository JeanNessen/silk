#pragma once

#include <array>
#include <cstdint>

namespace silk
{
enum class Side
{
    Buy,
    Sell,
    None,
};

struct AddOrder
{
    std::uint16_t       locate    = 0;
    std::uint16_t       tracking  = 0;
    std::uint64_t       timestamp = 0;
    std::uint64_t       refNum    = 0;
    Side                side      = Side::None;
    std::uint32_t       numShares = 0;
    std::array<char, 8> stock{0x00};
    std::int32_t        price = 0;
};

inline bool operator==(const AddOrder& lhs, const AddOrder& rhs)
{
    return lhs.locate == rhs.locate && lhs.tracking == rhs.tracking && lhs.timestamp == rhs.timestamp
           && lhs.refNum == rhs.refNum && lhs.side == rhs.side && lhs.numShares == rhs.numShares
           && lhs.stock == rhs.stock && lhs.price == rhs.price;
}
}  // namespace silk
