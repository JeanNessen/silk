#include "silk/parser/utils.h"

#include <cassert>
#include <ostream>
#include <string_view>
#include <utility>

#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "silk/parser/types.h"

namespace silk
{

std::string ToString(const Type type)
{
    switch (type)
    {
        case Type::Add:
            return "Add";
        case Type::AddMPID:
            return "AddMPID";
        case Type::Executed:
            return "Executed";
        case Type::ExecutedWithPrice:
            return "ExecutedWithPrice";
        case Type::Canceled:
            return "Canceled";
        case Type::Deleted:
            return "Deleted";
        case Type::Replaced:
            return "Replaced";
        case Type::None:
            return "None";
    }

    assert("Unexpected Type");
    return "Unexpected Type";
}

void PrintTo(const OrderHeader& header, std::ostream* os)
{
    *os << "\nlocate=" << header.locate << ", \ntracking=" << header.tracking
        << ", \ntimestamp=" << header.timestamp
        << ", \nrefNum=" << header.refNum;
}

void PrintTo(const OrderAdd& order, std::ostream* os)
{
    *os << "OrderAdd{\nheader={";
    PrintTo(order.header, os);
    *os << "}, \nside=" << std::to_underlying(order.side)
        << ", \nnumShares=" << order.numShares << ", \nstock="
        << std::string_view(order.stock.data(), order.stock.size())
        << ", \nprice=" << order.price << "}";
}

void PrintTo(const OrderExecuted& order, std::ostream* os)
{
    *os << "OrderExecuted{\nheader={";
    PrintTo(order.header, os);
    *os << "}, \nexecutedShares=" << order.executedShares
        << ", \nmatchNum=" << order.matchNum << "}";
}

void PrintTo(const OrderAddMPID& order, std::ostream* os)
{
    *os << "OrderAddMPID{\nadd=";
    PrintTo(order.add, os);
    *os << ", \nmpid=" << order.mpid << "}";
}

void PrintTo(const OrderReplaced& order, std::ostream* os)
{
    *os << "OrderReplaced{\nheader={";
    PrintTo(order.header, os);
    *os << "}, \nnewRefNum=" << order.newRefNum
        << ", \nshares=" << order.shares << ", \nprice=" << order.price << "}";
}


}  // namespace silk
