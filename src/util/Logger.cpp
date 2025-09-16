#include "util/Logger.h"
#include <iomanip>
#include <sstream>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : minLevel_(LogLevel::INFO) {
    // 默认输出到控制台
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::setLogFile(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(logMutex_);
    if (logFile_.is_open()) {
        logFile_.close();
    }
    logFile_.open(filePath, std::ios::app);
}

void Logger::setMinLevel(LogLevel level) {
    minLevel_ = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < minLevel_) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(logMutex_);
    std::stringstream ss;
    ss << getCurrentTime() << " [" << getLevelString(level) << "] " << message << std::endl;
    
    std::string logEntry = ss.str();
    std::cout << logEntry;
    
    if (logFile_.is_open()) {
        logFile_ << logEntry;
        logFile_.flush();
    }
}

std::string Logger::getLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}