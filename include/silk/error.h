
#include <cstdint>
#include <expected>
namespace silk
{
enum class ErrorCode : std::uint8_t
{
    OpenFailed,
};

struct Error
{
    ErrorCode code;
    int       errno = 0;
};

template <class T>
using Result = std::expected<T, Error>;

}  // namespace silk
