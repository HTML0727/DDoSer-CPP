#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <iostream>
#include <chrono>
#include <iomanip>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger& getInstance();
    
    void log(LogLevel level, const std::string& message);
    void setLogFile(const std::string& filePath);
    void setMinLevel(LogLevel level);
    
    // 禁止拷贝和移动
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;
    
private:
    Logger();
    ~Logger();
    
    std::ofstream logFile_;
    std::mutex logMutex_;
    LogLevel minLevel_{LogLevel::INFO};
    
    std::string getLevelString(LogLevel level) const;
    std::string getCurrentTime() const;
};