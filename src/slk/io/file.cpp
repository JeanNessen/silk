#include "file.h"

#include <expected>
#include <string>

#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>

namespace slk
{
MappedFile::MappedFile() {}

MappedFile::MappedFile(MappedFile&& other)
    : m_pAddress(other.m_pAddress)
    , m_len(other.m_len)
{
    other.m_pAddress = nullptr;
    other.m_len      = 0;
}

MappedFile& MappedFile::operator=(MappedFile&& other)
{
    if (other == *this)
    {
        return *this;
    }

    m_pAddress = other.m_pAddress;
    m_len      = other.m_len;

    other.m_pAddress = nullptr;
    other.m_len      = 0;

    return *this;
}

MappedFile::~MappedFile()
{
    munmap(m_pAddress, m_len);
}

bool MappedFile::operator==(const MappedFile& other) const
{
    return m_pAddress == other.m_pAddress && m_len == other.m_len;
}

Result<MappedFile> Open(std::string path)
{
    int fd = open(path.c_str(), O_RDONLY);

    if (fd == -1)
    {
        return std::unexpected(
            Error{.code = ErrorCode::OpenFailed, .no = errno});
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        close(fd);
        return std::unexpected(
            Error{.code = ErrorCode::OpenFailed, .no = errno});
    }

    MappedFile file;

    file.SetLen(static_cast<std::size_t>(sb.st_size));

    auto pAddress = reinterpret_cast<std::uint8_t*>(
        mmap(NULL, file.GetLen(), PROT_READ, MAP_PRIVATE, fd, 0));
    madvise(pAddress, file.GetLen(), MADV_POPULATE_READ);

    if (pAddress == MAP_FAILED)
    {
        close(fd);
        return std::unexpected(
            Error{.code = ErrorCode::OpenFailed, .no = errno});
    }

    file.SetAddress(pAddress);

    close(fd);


    return file;
}

}  // namespace slk
