#include "catch2/catch_all.hpp"
#include "Reader.hpp"

TEST_CASE("Reader")
{
    Reader reader;

    SECTION("Sanity")
    {
        auto result = reader.readConfig();
        REQUIRE(result);
        REQUIRE(reader.getDir1() == "/home/user/documents");
        REQUIRE(reader.getDir2() == "/home/user/downloads");
        REQUIRE(reader.getInterval() == 5);
    }
    SECTION("Rainy")
    {
        auto result = reader.readConfig("LALALALA");
        REQUIRE(!result);
        REQUIRE(result.error() == "Failed to open config file.");
    }
}
