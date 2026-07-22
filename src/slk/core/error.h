#pragma once

#include <cstdint>
#include <expected>

namespace slk
{
enum class ErrorCode : std::uint8_t
{
    OpenFailed,
};

struct Error
{
    ErrorCode code;
    int       no = 0;
};

template <class T>
using Result = std::expected<T, Error>;

}  // namespace slk
