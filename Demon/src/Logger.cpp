#include "Logger.hpp"
#include <syslog.h>

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger() {}

void Logger::openLog(std::string const& identifier) const
{
    openlog(identifier.c_str(), LOG_PID, LOG_DAEMON);
}

void Logger::logInfo(std::string const& message) const
{
    syslog(LOG_INFO, "%s", message.c_str());
}

void Logger::logError(std::string const& message) const
{
    syslog(LOG_ERR, "%s", message.c_str());
}

void Logger::closeLog() const
{
    closelog();
}
