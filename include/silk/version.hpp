#pragma once

#include <string_view>

namespace silk
{

// Populated from the project version defined in CMake.
inline constexpr std::string_view version = SILK_VERSION;

// Returns the compiled-in library version string.
std::string_view library_version() noexcept;

}  // namespace silk
