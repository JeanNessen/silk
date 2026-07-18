#include <cstdint>
#include <cstring>
#include <string_view>

#include <gtest/gtest.h>
#include <silk/parser/parser.h>
#include <silk/parser/types.h>
#include <silk/parser/utils.h>

constexpr std::uint8_t MessageAddBuy[36]{
    0x41, 0x10, 0x27, 0x00, 0x00, 0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, 0x42, 0x00, 0x00, 0x00, 0x64,
    0x41, 0x41, 0x50, 0x4c, 0x20, 0x20, 0x20, 0x20, 0x00, 0x12, 0xd6, 0x44,
};

constexpr std::uint8_t MessageAddBuyMPID[40]{
    0x46, 0x10, 0x27, 0x00, 0x00, 0x1f, 0x1a, 0xce, 0xd9, 0xf0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, 0x42,
    0x00, 0x00, 0x00, 0x64, 0x41, 0x41, 0x50, 0x4c, 0x20, 0x20,
    0x20, 0x20, 0x00, 0x12, 0xd6, 0x44, 0x00, 0x00, 0x30, 0x39,
};

constexpr std::uint8_t MessageHeader[19]{
    0x41, 0x10, 0x27, 0x00, 0x00, 0x1f, 0x1a, 0xce, 0xd9, 0xf0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66,
};

constexpr std::uint8_t MessageExecuted[]{
    0x45, 0x10, 0x27, 0x00, 0x00, 0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66, 0x00, 0x00, 0x00,
    0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
};

constexpr silk::OrderHeader Header{
    .locate    = 4135,
    .tracking  = 0,
    .timestamp = 34200000000000,
    .refNum    = 42086,
};

constexpr silk::OrderAdd AddBuy{
    .header    = Header,
    .side      = silk::Side::Buy,
    .numShares = 100,
    .stock     = {'A', 'A', 'P', 'L', ' ', ' ', ' ', ' '},
    .price     = 1234500,
};

constexpr silk::OrderAddMPID AddBuyMPID{
    .add  = AddBuy,
    .mpid = 12345,
};

constexpr silk::OrderExecuted Executed{
    .header         = Header,
    .executedShares = 9,
    .matchNum       = 3,
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

public:
    silk::OrderAdd               lastAdd;
    silk::OrderAddMPID           lastAddMPID;
    silk::OrderExecuted          lastExecuted;
    silk::OrderExecutedWithPrice lastExecutedWithPrice;
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
    silk::ParseMessage(MessageAddBuy, handler);
    EXPECT_EQ(handler.lastAdd, AddBuy);
}

TEST(Parser, HandlerAddMPID)
{
    TestHandler handler;
    silk::ParseMessage(MessageAddBuyMPID, handler);
    EXPECT_EQ(handler.lastAddMPID, AddBuyMPID);
}

TEST(Parser, HandlerExecuted)
{
    TestHandler handler;
    silk::ParseMessage(MessageExecuted, handler);
    EXPECT_EQ(handler.lastExecuted, Executed);
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
