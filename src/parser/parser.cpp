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

    return order;
}

}  // namespace silk
