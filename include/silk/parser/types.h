#pragma once

#include <array>
#include <cstdint>

namespace silk
{
enum class Side
{
    None = 0,
    Buy,
    Sell,
};

enum class Type
{
    None = 0,
    Add,
    AddMPID,
    Executed,
    ExecutedWithPrice,
    Canceled,
    Deleted,
    Replaced,
};

struct OrderHeader
{
    bool operator==(const OrderHeader&) const = default;

    std::uint16_t locate    = 0;
    std::uint16_t tracking  = 0;
    std::uint64_t timestamp = 0;
    std::uint64_t refNum    = 0;
};

struct OrderAdd
{
    bool operator==(const OrderAdd&) const = default;

    OrderHeader         header;
    Side                side      = Side::None;
    std::uint32_t       numShares = 0;
    std::array<char, 8> stock{0x00};
    std::int32_t        price = 0;
};

struct OrderAddMPID
{
    bool          operator==(const OrderAddMPID&) const = default;
    OrderAdd      add;
    std::uint32_t mpid = 0;
};

struct OrderExecuted
{
    bool operator==(const OrderExecuted&) const = default;

    OrderHeader   header;
    std::uint32_t executedShares = 0;
    std::uint64_t matchNum       = 0;
};

struct OrderExecutedWithPrice
{
    bool operator==(const OrderExecutedWithPrice&) const = default;

    OrderExecuted executed;
    bool          printable      = false;
    std::uint32_t executionPrice = 0;
};

struct OrderCanceled
{
    bool          operator==(const OrderCanceled&) const = default;
    OrderHeader   header;
    std::uint32_t canceledShares = 0;
};

struct OrderDeleted
{
    bool        operator==(const OrderDeleted&) const = default;
    OrderHeader header;
};

struct OrderReplaced
{
    bool          operator==(const OrderReplaced&) const = default;
    OrderHeader   header;
    std::uint64_t newRefNum = 0;
    std::uint32_t shares    = 0;
    std::uint32_t price     = 0;
};

constexpr std::size_t LenMessageLen     = 2;
constexpr std::size_t LenType           = 1;
constexpr std::size_t LenLocate         = 2;
constexpr std::size_t LenTracking       = 2;
constexpr std::size_t LenTimestamp      = 6;
constexpr std::size_t LenRefNum         = 8;
constexpr std::size_t LenSide           = 1;
constexpr std::size_t LenShares         = 4;
constexpr std::size_t LenStock          = 8;
constexpr std::size_t LenPrice          = 4;
constexpr std::size_t LenMPID           = 4;
constexpr std::size_t LenExecutedShares = 4;
constexpr std::size_t LenMatchNum       = 8;
constexpr std::size_t LenPrintable      = 1;
constexpr std::size_t LenExecutionPrice = 4;
constexpr std::size_t LenCanceledShares = 4;

}  // namespace silk
