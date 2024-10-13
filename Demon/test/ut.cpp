#include "catch2/catch_all.hpp"
#include "Reader.hpp"
#include "Demon.hpp"
#include <fstream>
#include <sys/stat.h>

TEST_CASE("Reader read config")
{
    Reader& reader = Reader::getInstance();

    SECTION("Sanity")
    {
        auto result = reader.readConfig();
        REQUIRE(result);
        REQUIRE(reader.getDir1() == "/home/daniil/Desktop/test1");
        REQUIRE(reader.getDir2() == "/home/daniil/Desktop/test2");
        REQUIRE(reader.getInterval() == 30);
    }
    SECTION("Rainy")
    {
        auto result = reader.readConfig("LALALALA");
        REQUIRE(!result);
        REQUIRE(result.error() == "Failed to open config file.");
    }
}

TEST_CASE("Demon monitor writes logs correctly")
{
    std::string testDir = "/tmp/testDir";
    mkdir(testDir.c_str(), 0777);

    std::ofstream(testDir + "/file1.txt").close();
    std::ofstream(testDir + "/file2.txt").close();

    static std::string const TMP_LOG_FILE{"/tmp/test_log.log"};

    Demon& demon = Demon::getInstance();
    demon.monitor(testDir, TMP_LOG_FILE);

    std::ifstream log(TMP_LOG_FILE);
    REQUIRE(log.is_open());

    std::string line;
    bool foundFile1 = false;
    bool foundFile2 = false;

    while (std::getline(log, line))
    {
        if (line.find("file1.txt") != std::string::npos)
        {
            foundFile1 = true;
        }
        if (line.find("file2.txt") != std::string::npos)
        {
            foundFile2 = true;
        }
    }

    REQUIRE(foundFile1);
    REQUIRE(foundFile2);

    // Remove tmp files
    unlink((testDir + "/file1.txt").c_str());
    unlink((testDir + "/file2.txt").c_str());
    unlink(TMP_LOG_FILE.c_str());
    rmdir(TMP_LOG_FILE.c_str());
}
