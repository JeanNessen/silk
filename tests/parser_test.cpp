#include <cstdint>
#include <cstring>
#include <string_view>

#include <gtest/gtest.h>
#include <slk/core/types.h>
#include <slk/parser/parser.h>

#include "order_printers.h"

// clang-format off
constexpr std::array<std::uint8_t, 36> MessageAdd{
    0x41,                                           // Message Type
    0x10, 0x27,                                     // Stock Locate
    0x00, 0x00,                                     // Tracking Number
    0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,             // Timestamp
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, // Reference Number

    0x42,                                           // Buy/Sell Indicator
    0x00, 0x00, 0x00, 0x64,                         // Shares
    0x41, 0x41, 0x50, 0x4c, 0x20, 0x20, 0x20, 0x20, // Stock
    0x00, 0x12, 0xd6, 0x44,                         // Price
};

constexpr std::array<std::uint8_t, 40> MessageAddMPID{
    0x46,                                           // Message Type
    0x10, 0x27,                                     // Stock Locate
    0x00, 0x00,                                     // Tracking Number
    0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,             // Timestamp
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, // Reference Number

    0x42,                                           // Buy/Sell Indicator
    0x00, 0x00, 0x00, 0x64,                         // Shares
    0x41, 0x41, 0x50, 0x4c, 0x20, 0x20, 0x20, 0x20, // Stock
    0x00, 0x12, 0xd6, 0x44,                         // Price
    0x00, 0x00, 0x30, 0x39,                         // Attribution
};

constexpr std::array<std::uint8_t, 19> MessageHeader{
    0x41,                                           // Message Type
    0x10, 0x27,                                     // Stock Locate
    0x00, 0x00,                                     // Tracking Number
    0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,             // Timestamp
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, // Reference Number
};

constexpr std::array<std::uint8_t, 31> MessageExecuted{
    0x45,                                           // Message Type
    0x10, 0x27,                                     // Stock Locate
    0x00, 0x00,                                     // Tracking Number
    0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,             // Timestamp
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, // Reference Number

    0x00, 0x00, 0x00, 0x09,                         // Executed Shares
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, // Match Number
};

constexpr std::array<std::uint8_t, 36> MessageExecutedWithPrice{
    0x43,                                           // Message Type
    0x10, 0x27,                                     // Stock Locate
    0x00, 0x00,                                     // Tracking Number
    0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,             // Timestamp
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, // Reference Number

    0x00, 0x00, 0x00, 0x09,                         // Executed Shares
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, // Match Number
    0x59,                                           // Printable
    0x00, 0x00, 0x00, 0x5A,                         // Price
};

constexpr std::array<std::uint8_t, 23> MessageCanceled{
    0x58,                                           // Message Type
    0x10, 0x27,                                     // Stock Locate
    0x00, 0x00,                                     // Tracking Number
    0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,             // Timestamp
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, // Reference Number

    0x00, 0x00, 0x00, 0x32,                         // Canceled Shares
};

constexpr std::array<std::uint8_t, 19> MessageDeleted{
    0x44,                                           // Message Type
    0x10, 0x27,                                     // Stock Locate
    0x00, 0x00,                                     // Tracking Number
    0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,             // Timestamp
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, // Reference Number
};

constexpr std::array<std::uint8_t, 35> MessageReplaced{
    0x55,                                           // Message Type
    0x10, 0x27,                                     // Stock Locate
    0x00, 0x00,                                     // Tracking Number
    0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,             // Timestamp
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, // Reference Number

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, // New Order Reference
    0x00, 0x00, 0x03, 0x0B,                         // Shares
    0x00, 0x00, 0x00, 0x0B,                         // Price
};
// clang-format on

constexpr silk::OrderHeader Header{
    .locate    = 4135,
    .tracking  = 0,
    .timestamp = 34200000000000,
    .refNum    = 42086,
};

constexpr silk::OrderAdd Add{
    .header    = Header,
    .side      = silk::Side::Buy,
    .numShares = 100,
    .stock     = {'A', 'A', 'P', 'L', ' ', ' ', ' ', ' '},
    .price     = 1234500,
};

constexpr silk::OrderAddMPID AddMPID{
    .add  = Add,
    .mpid = 12345,
};

constexpr silk::OrderExecuted Executed{
    .header         = Header,
    .executedShares = 9,
    .matchNum       = 3,
};

constexpr silk::OrderExecutedWithPrice ExecutedWithPrice{
    .executed       = Executed,
    .printable      = true,
    .executionPrice = 90,
};

constexpr silk::OrderCanceled Canceled{
    .header         = Header,
    .canceledShares = 50,
};

constexpr silk::OrderDeleted Deleted{
    .header = Header,
};

constexpr silk::OrderReplaced Replaced{
    .header    = Header,
    .newRefNum = 89,
    .shares    = 779,
    .price     = 11,
};

class TestHandler final
{
public:
    void OnAdd(silk::OrderAdd order) { lastAdd = order; }
    void OnAddMPID(silk::OrderAddMPID order) { lastAddMPID = order; }
    void OnExecuted(silk::OrderExecuted order) { lastExecuted = order; }
    void OnExecutedWithPrice(silk::OrderExecutedWithPrice order)
    {
        lastExecutedWithPrice = order;
    }
    void OnCanceled(silk::OrderCanceled order) { lastCanceled = order; }
    void OnDeleted(silk::OrderDeleted order) { lastDeleted = order; }
    void OnReplaced(silk::OrderReplaced order) { lastReplaced = order; }
    void OnSkipped() {};

public:
    silk::OrderAdd               lastAdd;
    silk::OrderAddMPID           lastAddMPID;
    silk::OrderExecuted          lastExecuted;
    silk::OrderExecutedWithPrice lastExecutedWithPrice;
    silk::OrderCanceled          lastCanceled;
    silk::OrderDeleted           lastDeleted;
    silk::OrderReplaced          lastReplaced;
};

TEST(Parser, Header)
{
    auto* pCursor = &MessageHeader[1];
    auto  header  = silk::BuildHeader(pCursor);
    EXPECT_EQ(header, Header);
}

TEST(Parser, HandlerAdd)
{
    TestHandler handler;
    silk::ParseMessage(MessageAdd.data(), handler);
    EXPECT_EQ(handler.lastAdd, Add);
}

TEST(Parser, HandlerAddMPID)
{
    TestHandler handler;
    silk::ParseMessage(MessageAddMPID.data(), handler);
    EXPECT_EQ(handler.lastAddMPID, AddMPID);
}

TEST(Parser, HandlerExecuted)
{
    TestHandler handler;
    silk::ParseMessage(MessageExecuted.data(), handler);
    EXPECT_EQ(handler.lastExecuted, Executed);
}

TEST(Parser, HandlerExecutedWithPrice)
{
    TestHandler handler;
    silk::ParseMessage(MessageExecutedWithPrice.data(), handler);
    EXPECT_EQ(handler.lastExecutedWithPrice, ExecutedWithPrice);
}

TEST(Parser, HandlerCanceled)
{
    TestHandler handler;
    silk::ParseMessage(MessageCanceled.data(), handler);
    EXPECT_EQ(handler.lastCanceled, Canceled);
}

TEST(Parser, HandlerDeleted)
{
    TestHandler handler;
    silk::ParseMessage(MessageDeleted.data(), handler);
    EXPECT_EQ(handler.lastDeleted, Deleted);
}

TEST(Parser, HandlerReplaced)
{
    TestHandler handler;
    silk::ParseMessage(MessageReplaced.data(), handler);
    EXPECT_EQ(handler.lastReplaced, Replaced);
}

TEST(Parser, Uint8)
{
    std::uint8_t message[1]{0x40};

    EXPECT_EQ(silk::ParseValue<std::uint8_t>(message), 64);
}

TEST(Parser, Uint16)
{
    std::uint8_t message[2]{0x00, 0x04};

    EXPECT_EQ(silk::ParseValue<std::uint16_t>(message), 4);
}

TEST(Parser, Uint32)
{
    std::uint8_t message[4]{0x00, 0x01, 0x00, 0x00};

    EXPECT_EQ(silk::ParseValue<std::uint32_t>(message), 65536);
}

TEST(Parser, Int32)
{
    std::uint8_t message[4]{0x00, 0x01, 0x00, 0x00};

    EXPECT_EQ(silk::ParseValue<std::int32_t>(message), 65536);
}

TEST(Parser, Uint64)
{
    std::uint8_t message[8]{0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    EXPECT_EQ(silk::ParseValue<std::uint64_t>(message), 281474976710656);
}

TEST(Parser, Char)
{
    std::uint8_t message[1]{0x41};

    EXPECT_EQ(silk::ParseValue<char>(message), 'A');
}

TEST(Parser, String)
{
    std::uint8_t message[8]{
        0x41, 0x41, 0x50, 0x4c, 0x20, 0x20, 0x20, 0x20,
    };

    auto s = silk::ParseValue<char, 8>(message);

    EXPECT_EQ(std::string_view(s.data(), s.size()), "AAPL    ");
}

TEST(Parser, SmallerInt)
{
    std::uint8_t message[2]{0x01, 0x01};

    EXPECT_EQ((silk::ParseValue<std::uint32_t, 2>(message)), 257);
}

TEST(Parser, Timestamp)
{
    std::uint8_t message[6]{
        0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,
    };

    EXPECT_EQ((silk::ParseValue<std::uint64_t, 6>(message)), 34200000000000);
}

TEST(Parser, Type)
{
    std::uint8_t message[1]{
        0x41,
    };

    EXPECT_EQ((silk::ParseValue<silk::Type, silk::LenType>(message)),
              silk::Type::Add);
}
