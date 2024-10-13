#pragma once

#include <string>

inline const char* PID_FILE = "/var/run/Demon.pid";

class Reader;
class Logger;

class Demon
{
public:
    static Demon& getInstance();
    void start();
    void monitor(std::string const& dirPath, std::string const& logFile);
    bool isAlreadyRunning() const;

private:
    Demon();
    ~Demon();
    Demon(Demon const&) = delete;
    Demon& operator=(Demon const&) = delete;

    void demonize();
    void handleSignals();
    void signalHandler(int signal);

    void run();

    static void sighupHandler(int signum);
    static void sigtermHandler(int signum);
    static void shutDown();

    Reader& m_reader;
    Logger& m_logger;
};
