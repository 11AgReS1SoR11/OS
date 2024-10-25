#pragma once

#include <string>

class Logger
{
public:
    static Logger& getInstance();

    void openLog(std::string const& identifier) const;
    void logInfo(std::string const& message) const;
    void logError(std::string const& message) const;
    void closeLog() const;

private:
    Logger();
    Logger(Logger const&) = delete;
    Logger& operator=(Logger const&) = delete;
};
