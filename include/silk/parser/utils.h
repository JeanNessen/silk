#pragma once

#include <ostream>
#include <string>

#include "types.h"

namespace silk
{

std::string ToString(const Type type);

void PrintTo(const OrderHeader& header, std::ostream* os);
void PrintTo(const OrderAdd& order, std::ostream* os);
void PrintTo(const OrderAddMPID& order, std::ostream* os);
void PrintTo(const OrderExecuted& order, std::ostream* os);

}  // namespace silk
