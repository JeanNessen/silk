#pragma once

#include <cstdint>

#include <slk/core/types.h>
#include <slk/io/file.h>
#include <slk/parser/parser.h>

namespace silk
{

template <typename Handler>
void ParseFile(MappedFile& file, Handler& h)
{
    const std::uint8_t* pCursor = file.GetAddress();
    const std::uint8_t* pEnd    = file.GetAddress() + file.GetLen();

    while (pCursor + 2 != pEnd)
    {
        std::uint16_t messageLen =
            Extract<std::uint16_t, LenMessageLen>(pCursor);

        if (pCursor + 2 + messageLen > pEnd)
        {
            break;
        }

        ParseMessage(pCursor, h);
        pCursor += messageLen;
    }
}

}  // namespace silk
