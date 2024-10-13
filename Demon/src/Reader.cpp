#include "Reader.hpp"
#include <fstream>

Reader& Reader::getInstance()
{
    static Reader instance;
    return instance;
}

std::expected<void, std::string> Reader::readConfig(std::string const& configPath)
{
    if (m_configPath.empty())
    {
        char absPath[4096];
        if (realpath(configPath.c_str(), absPath) == nullptr)
        {
            return std::unexpected{"Failed to get absolute path of config file."};
        }
        m_configPath = absPath;
    }

    std::ifstream configFile(m_configPath);
    if (!configFile) { return std::unexpected{"Failed to open config file."}; }

    std::getline(configFile, m_dir1);
    std::getline(configFile, m_dir2);
    configFile >> m_interval;

    return {};
}
