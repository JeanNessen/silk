#pragma once

#include <array>
#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "types.h"

namespace silk
{

template <std::integral T, size_t size = sizeof(T)>
inline auto ParseValue(const uint8_t* pMessage)
{
    if constexpr (std::same_as<T, char> && size > 1)
    {
        std::array<char, size> buf{};
        std::memcpy(buf.data(), pMessage, size);
        return buf;
    }
    else if constexpr (std::same_as<T, bool>)
    {
        char t{};
        std::memcpy(&t, pMessage, sizeof(char));
        switch (t)
        {
            case 'Y':
                return true;
            case 'N':
                return false;
            default:
                return false;
        }
    }
    else
    {
        T value{};
        // In the case size < sizeof(T) we want to copy into the high bytes
        // before byteswapping
        // TODO: In the case where size is not a full width read (i.e. 6 bytes
        // for a timestamp) the memcpy compiles into multiple reads.
        // Optimization would be to overread, but this requires pMessage to be
        // at least sizeof(T) long. Some compiler might already do this anyway
        std::memcpy(reinterpret_cast<uint8_t*>(&value) + (sizeof(T) - size),
                    pMessage, size);
        if constexpr (std::endian::native == std::endian::little)
        {
            value = std::byteswap(value);
        }
        return value;
    }
}

template <std::same_as<Side> T, std::size_t size>
inline Side ParseValue(const uint8_t* pMessage)
{
    char t{};
    std::memcpy(&t, pMessage, sizeof(char));
    switch (t)
    {
        case 'B':
            return Side::Buy;
        case 'S':
            return Side::Sell;
        default:
            return Side::None;
    }
}

template <std::same_as<Type> T, std::size_t size>
inline Type ParseValue(const uint8_t* pMessage)
{
    char t{};
    std::memcpy(&t, pMessage, sizeof(char));
    switch (t)
    {
        case 'A':
            return Type::Add;
        case 'F':
            return Type::AddMPID;
        case 'E':
            return Type::Executed;
        case 'C':
            return Type::ExecutedWithPrice;
        case 'X':
            return Type::Canceled;
        case 'D':
            return Type::Deleted;
        case 'U':
            return Type::Replaced;
        default:
            return Type::None;
    }
}

template <typename T, std::size_t size>
inline auto Extract(const std::uint8_t*& pCursor)
{
    auto t = ParseValue<T, size>(pCursor);
    pCursor += size;
    return t;
}

inline OrderHeader BuildHeader(const std::uint8_t*& pCursor)
{
    return {
        .locate    = Extract<std::uint16_t, LenLocate>(pCursor),
        .tracking  = Extract<std::uint16_t, LenTracking>(pCursor),
        .timestamp = Extract<std::uint64_t, LenTimestamp>(pCursor),
        .refNum    = Extract<std::uint64_t, LenRefNum>(pCursor),
    };
}

inline OrderAdd BuildAdd(const std::uint8_t*& pCursor)
{
    return {
        .header    = BuildHeader(pCursor),
        .side      = Extract<Side, LenSide>(pCursor),
        .numShares = Extract<std::uint32_t, LenShares>(pCursor),
        .stock     = Extract<char, LenStock>(pCursor),
        .price     = Extract<std::int32_t, LenPrice>(pCursor),
    };
}

inline OrderExecuted BuildExecuted(const std::uint8_t*& pCursor)
{
    return {
        .header         = BuildHeader(pCursor),
        .executedShares = Extract<std::uint32_t, LenExecutedShares>(pCursor),
        .matchNum       = Extract<std::uint64_t, LenMatchNum>(pCursor),
    };
}

template <typename Handler>
void ParseMessage(const std::uint8_t* pMessage, Handler& h)
{
    const std::uint8_t* pCursor = pMessage;

    auto type = Extract<Type, LenType>(pCursor);

    switch (type)
    {
        case Type::Add:
            h.OnAdd(BuildAdd(pCursor));
            break;
        case Type::AddMPID:
            h.OnAddMPID({
                .add  = BuildAdd(pCursor),
                .mpid = Extract<std::uint32_t, LenMPID>(pCursor),
            });
            break;
        case Type::Executed:
            h.OnExecuted(BuildExecuted(pCursor));
            break;
        case Type::ExecutedWithPrice:
            h.OnExecutedWithPrice({
                .executed  = BuildExecuted(pCursor),
                .printable = Extract<bool, LenPrintable>(pCursor),
                .executionPrice =
                    Extract<std::uint32_t, LenExecutionPrice>(pCursor),
            });
            break;
        case Type::Canceled:
        case Type::Deleted:
        case Type::Replaced:
        case Type::None:
            break;
    }
}

}  // namespace silk
