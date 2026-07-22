#include <cstring>
#include <fstream>

#include <gtest/gtest.h>
#include <slk/io/file.h>

TEST(File, load)
{
    // Create the file
    std::ofstream file("test");
    file << "HALLO";
    file.close();

    auto result = silk::Open("test");
    EXPECT_TRUE(result.has_value());

    auto pAddress = result->GetAddress();
    char read;
    std::memcpy(&read, pAddress + 1, 1);

    EXPECT_EQ(read, 'A');

    std::remove("test");
}
