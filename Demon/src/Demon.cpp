#include "Demon.hpp"
#include "Logger.hpp"
#include "Reader.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <cstring>
#include <dirent.h>
#include <ctime>

static std::string const HIST_FILE{"/hist.log"};

Demon& Demon::getInstance()
{
    static Demon instance;
    return instance;
}

Demon::Demon() : m_reader(Reader::getInstance()), m_logger(Logger::getInstance()) {}

Demon::~Demon() { /*shutDown();*/ }

void Demon::start()
{
    Logger::getInstance().logInfo("Demon start.");
    if (isAlreadyRunning())
    {
        m_logger.logError("Demon is already running.");
        exit(EXIT_FAILURE);
    }

    Logger::getInstance().logInfo("Demon not isAlreadyRunning.");

    if (auto result = m_reader.readConfig(); !result)
    {
        m_logger.logError(result.error());
        exit(EXIT_FAILURE);
    }

    Logger::getInstance().logInfo("Demon reads config successful.");

    demonize();
    Logger::getInstance().logInfo("Demon demonize successful.");
    handleSignals();
    Logger::getInstance().logInfo("Demon handleSignals successful.");
    run();
}

void Demon::demonize()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        Logger::getInstance().logInfo("Demon has PID < 0.");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        Logger::getInstance().logInfo("Demon has PID > 0.");
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0)
    {
        Logger::getInstance().logInfo("Demon setsid fail.");
        exit(EXIT_FAILURE);
    }

    signal(SIGHUP, SIG_IGN);
    pid = fork();
    if (pid < 0)
    {
        Logger::getInstance().logInfo("Demon has PID < 0. [2]");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        Logger::getInstance().logInfo("Demon has PID > 0.");
        exit(EXIT_SUCCESS);
    }

    umask(0);
    chdir("/");

    Logger::getInstance().logInfo("Demon umask(0) and chdir() success.");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    Logger::getInstance().logInfo("Demon close descryptors success.");

    m_logger.openLog("Demon");

    Logger::getInstance().logInfo("Demon open log success.");

    if (std::ofstream pidFile(PID_FILE); pidFile)
    {
        pidFile << getpid() << std::endl;
    }
    else
    {
        Logger::getInstance().logInfo("Failed to create PID file.");
        m_logger.logError("Failed to create PID file.");
        exit(EXIT_FAILURE);
    }

    Logger::getInstance().logInfo("Demon demonize success.");
}

bool Demon::isAlreadyRunning() const
{
    if (std::ifstream pidFile(PID_FILE); pidFile)
    {
        pid_t pid;
        pidFile >> pid;
        if (kill(pid, 0) == 0)
        {
            kill(pid, SIGTERM);
            return true;
        }
    }
    return false;
}

void Demon::handleSignals()
{
    signal(SIGHUP, sighupHandler);
    signal(SIGTERM, sigtermHandler);
}

void Demon::sighupHandler(int signum)
{
    Logger::getInstance().logInfo("SIGHUP received, re-reading config.");
    if (auto result = Reader::getInstance().readConfig(); !result)
    {
        Logger::getInstance().logError(result.error());
        exit(EXIT_FAILURE);
    }
}

void Demon::shutDown()
{
    Logger::getInstance().logInfo("!!!Shut Down Demon!!!");
    unlink(PID_FILE);
    Logger::getInstance().closeLog();
    exit(0);
}

void Demon::sigtermHandler(int signum)
{
    Logger::getInstance().logInfo("SIGTERM received, terminating.");
    shutDown();
}

void Demon::run()
{
    while (true)
    {
        Logger::getInstance().logInfo("Demon monitoring.");
        monitor(m_reader.getDir1(), m_reader.getDir2() + HIST_FILE);
        Logger::getInstance().logInfo("Demon monitoring successful.");
        sleep(m_reader.getInterval());
    }
}

void Demon::monitor(std::string const& dirPath, std::string const& logFile)
{
    DIR* dir = opendir(dirPath.c_str());
    if (!dir)
    {
        m_logger.logError("Failed to open directory: " + dirPath);
        return;
    }

    std::ofstream log(logFile, std::ios_base::app);
    if (!log)
    {
        m_logger.logError("Failed to open log file: " + logFile);
        closedir(dir);
        return;
    }

    time_t now = time(nullptr);
    log << "Directory snapshot at: " << ctime(&now) << std::endl;

    while (auto* entry = readdir(dir))
    {
        log << entry->d_name << std::endl;
    }

    log << std::endl;
    closedir(dir);
}
