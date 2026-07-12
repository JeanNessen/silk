#pragma once

#include <cstdint>

namespace silk
{

struct AddOrder;

class Parser
{
public:
    AddOrder ParseAdd(const std::uint8_t message[36]) const;

private:
};
}  // namespace silk
