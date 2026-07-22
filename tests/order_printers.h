#pragma once

#include <ostream>

#include <slk/core/types.h>

namespace slk
{

void PrintTo(const OrderHeader& header, std::ostream* os);
void PrintTo(const OrderAdd& order, std::ostream* os);
void PrintTo(const OrderAddMPID& order, std::ostream* os);
void PrintTo(const OrderExecuted& order, std::ostream* os);
void PrintTo(const OrderReplaced& order, std::ostream* os);

}  // namespace slk
