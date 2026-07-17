#include <cstdint>
#include <cstring>
#include <string_view>

#include <gtest/gtest.h>
#include <silk/parser/parser.h>
#include <silk/parser/types.h>

constexpr std::uint8_t TestMessageAddBuy[36]{
    0x41,                                            // [0]  type 'A'
    0x10, 0x27,                                      // [1]  stock locate 4135  (BE)
    0x00, 0x00,                                      // [3]  tracking 0
    0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,              // [5]  timestamp 34199999938560 (48-bit)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x66,  // [11] order ref 42086
    0x42,                                            // [19] 'B' buy
    0x00, 0x00, 0x00, 0x64,                          // [20] shares 100
    0x41, 0x41, 0x50, 0x4c, 0x20, 0x20, 0x20, 0x20,  // [24] "AAPL    "
    0x00, 0x12, 0xd6, 0x44,                          // [32] price 1234500 → $123.4500
};

constexpr silk::AddOrder TestOrderAddBuy{
    .locate    = 4135,
    .tracking  = 0,
    .timestamp = 34200000000000,
    .refNum    = 42086,
    .side      = silk::Side::Buy,
    .numShares = 100,
    .stock     = {'A', 'A', 'P', 'L', ' ', ' ', ' ', ' '},
    .price     = 1234500,
};

TEST(Parser, DiscardNonAdd)
{
    std::uint8_t message[36]{};
    message[0] = 0x42;  // type 'B'
                        //
    silk::Parser parser;

    // Order that are not of type 'A' should return invalid AddOrder object
    EXPECT_EQ(parser.ParseAdd(&message[0]), silk::AddOrder{});
}

TEST(Parser, BasicAddOrder)
{
    silk::Parser parser;

    EXPECT_EQ(parser.ParseAdd(&TestMessageAddBuy[0]), TestOrderAddBuy);
}

TEST(Parser, ParseUint8)
{
    std::uint8_t message[1]{0x40};

    EXPECT_EQ(silk::ParseValue<std::uint8_t>(message), 64);
}

TEST(Parser, ParseUint16)
{
    std::uint8_t message[2]{0x00, 0x04};

    EXPECT_EQ(silk::ParseValue<std::uint16_t>(message), 4);
}

TEST(Parser, ParseUint32)
{
    std::uint8_t message[4]{0x00, 0x01, 0x00, 0x00};

    EXPECT_EQ(silk::ParseValue<std::uint32_t>(message), 65536);
}

TEST(Parser, ParseInt32)
{
    std::uint8_t message[4]{0x00, 0x01, 0x00, 0x00};

    EXPECT_EQ(silk::ParseValue<std::int32_t>(message), 65536);
}

TEST(Parser, ParseUint64)
{
    std::uint8_t message[8]{0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    EXPECT_EQ(silk::ParseValue<std::uint64_t>(message), 281474976710656);
}

TEST(Parser, ParseChar)
{
    std::uint8_t message[1]{0x41};

    EXPECT_EQ(silk::ParseValue<char>(message), 'A');
}

TEST(Parser, ParseString)
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

TEST(Parser, ParseTimestamp)
{
    std::uint8_t message[6]{
        0x1f, 0x1a, 0xce, 0xd9, 0xf0, 0x00,
    };

    EXPECT_EQ((silk::ParseValue<std::uint64_t, 6>(message)), 34200000000000);
}
