#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include <silk/error.h>

namespace silk
{

// This class wraps a file mapped via mmap
class MappedFile final
{
public:
    MappedFile();
    MappedFile(MappedFile&& other);
    MappedFile(const MappedFile&) = delete;
    MappedFile& operator=(MappedFile&& other);
    MappedFile& operator=(const MappedFile&) = delete;
    ~MappedFile();

    void        SetLen(std::size_t newLen) { m_len = newLen; }
    std::size_t GetLen() const { return m_len; }

    void SetAddress(std::uint8_t* pNewAddress) { m_pAddress = pNewAddress; }
    std::uint8_t* GetAddress() { return m_pAddress; }


private:
    bool operator==(const MappedFile& other) const;

private:
    std::uint8_t* m_pAddress = nullptr;
    std::size_t   m_len      = 0;
};

Result<MappedFile> Open(std::string path);

}  // namespace silk
