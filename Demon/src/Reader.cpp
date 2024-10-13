#include "Reader.hpp"
#include <fstream>

std::expected<void, std::string> Reader::readConfig(std::string_view configPath)
{
    std::string const f{configPath};
    std::ifstream configFile(f);
    if (!configFile) { return std::unexpected{"Failed to open config file."}; }

    m_configPath = configPath; // TODO: get absolute path
    std::getline(configFile, m_dir1);
    std::getline(configFile, m_dir2);
    configFile >> m_interval;

    return {};
}
