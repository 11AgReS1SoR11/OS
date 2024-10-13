#pragma once

#include <string>

class Logger
{
public:
    static Logger& getInstance();

    void openLog(std::string const& identifier);
    void logInfo(std::string const& message);
    void logError(std::string const& message);
    void closeLog();

private:
    Logger();
    Logger(Logger const&) = delete;
    Logger& operator=(Logger const&) = delete;
};
