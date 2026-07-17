#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <utility>

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

inline void PrintTo(const AddOrder& order, std::ostream* os)
{
    *os << "Order{\nlocate=" << order.locate << ", \ntracking=" << order.tracking << ", \ntimestamp=" << order.timestamp
        << ", \nrefNum=" << order.refNum << ", \nside=" << std::to_underlying(order.side)
        << ", \nnumShares=" << order.numShares
        << ", \nstock=" << std::string_view(order.stock.data(), order.stock.size()) << ", \nprice=" << order.price
        << "}";
}

// Offsets based on the spec
constexpr std::size_t OffType = 0;
constexpr std::size_t LenType = 1;

constexpr std::size_t OffLocate = OffType + LenType;
constexpr std::size_t LenLocate = 2;

constexpr std::size_t OffTracking = OffLocate + LenLocate;
constexpr std::size_t LenTracking = 2;

constexpr std::size_t OffTimestamp = OffTracking + LenTracking;
constexpr std::size_t LenTimestamp = 6;

constexpr std::size_t OffRefNum = OffTimestamp + LenTimestamp;
constexpr std::size_t LenRefNum = 8;

constexpr std::size_t OffSide = OffRefNum + LenRefNum;
constexpr std::size_t LenSide = 1;

constexpr std::size_t OffNumShares = OffSide + LenSide;
constexpr std::size_t LenNumShares = 4;

constexpr std::size_t OffStock = OffNumShares + LenNumShares;
constexpr std::size_t LenStock = 8;

constexpr std::size_t OffPrice = OffStock + LenStock;
constexpr std::size_t LenPrice = 4;

static_assert(OffLocate == 1);
static_assert(OffTracking == 3);
static_assert(OffTimestamp == 5);
static_assert(OffRefNum == 11);
static_assert(OffSide == 19);
static_assert(OffNumShares == 20);
static_assert(OffStock == 24);
static_assert(OffPrice == 32);


inline bool operator==(const AddOrder& lhs, const AddOrder& rhs)
{
    return lhs.locate == rhs.locate && lhs.tracking == rhs.tracking && lhs.timestamp == rhs.timestamp
           && lhs.refNum == rhs.refNum && lhs.side == rhs.side && lhs.numShares == rhs.numShares
           && lhs.stock == rhs.stock && lhs.price == rhs.price;
}
}  // namespace silk
