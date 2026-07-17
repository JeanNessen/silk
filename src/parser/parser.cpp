#include <cstdint>
#include <cstring>

#include <silk/parser/parser.h>
#include <silk/parser/types.h>

namespace silk
{
AddOrder Parser::ParseAdd(const std::uint8_t message[36]) const
{
    AddOrder order{};

    if (message[0] != 0x41)
    {
        return order;
    }

    order.locate    = ParseValue<std::uint16_t>(message + OffLocate);
    order.tracking  = ParseValue<std::uint16_t>(message + OffTracking);
    order.timestamp = ParseValue<std::uint64_t, LenTimestamp>(message + OffTimestamp);
    order.refNum    = ParseValue<std::uint64_t>(message + OffRefNum);
    order.numShares = ParseValue<std::uint32_t>(message + OffNumShares);
    order.stock     = ParseValue<char, LenStock>(message + OffStock);
    order.price     = ParseValue<std::int32_t>(message + OffPrice);

    const char side = ParseValue<char>(message + OffSide);
    switch (side)
    {
        case 'B':
            order.side = Side::Buy;
            break;
        case 'S':
            order.side = Side::Sell;
            break;
        default:
            break;
    }

    return order;
}

}  // namespace silk
