#pragma once

#include <expected>
#include <string_view>
#include <string>

inline constexpr std::string_view CONFIG_PATH = "/home/daniil/Desktop/Daniil/OS/Demon/config.txt"; // "../../Demon/config.txt";

class Reader
{
public:
    std::expected<void, std::string> readConfig(std::string_view configPath = CONFIG_PATH);

    std::string_view getDir1() const noexcept { return m_dir1; }
    std::string_view getDir2() const noexcept { return m_dir2; }
    int getInterval() const noexcept { return m_interval; }

private:
    std::string m_configPath;
    std::string m_dir1;
    std::string m_dir2;
    int m_interval;
};
