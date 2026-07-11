#include <gtest/gtest.h>

#include "silk/version.hpp"

// A trivial smoke test proving the library links and the test harness runs.
// Replace with real tests as functionality lands.
TEST(Version, IsNotEmpty)
{
    EXPECT_FALSE(silk::library_version().empty());
}

TEST(Version, MatchesHeaderConstant)
{
    EXPECT_EQ(silk::library_version(), silk::version);
}
