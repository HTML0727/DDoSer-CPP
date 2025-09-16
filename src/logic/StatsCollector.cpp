#include "logic/StatsCollector.h"
#include <chrono>

StatsCollector& StatsCollector::getInstance() {
    static StatsCollector instance;
    return instance;
}

StatsCollector::StatsCollector() : collecting_(false) {}

void StatsCollector::startCollection() {
    if (collecting_) return;
    
    stats_ = AttackStats();
    stats_.startTime = std::chrono::steady_clock::now();
    lastUpdate_ = stats_.startTime;
    collecting_ = true;
    
    // 启动统计更新线程
    std::thread([this]() {
        while (collecting_) {
            updateRates();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }).detach();
}

void StatsCollector::stopCollection() {
    collecting_ = false;
    stats_.endTime = std::chrono::steady_clock::now();
}

void StatsCollector::reset() {
    stats_ = AttackStats();
    lastUpdate_ = std::chrono::steady_clock::now();
}

void StatsCollector::incrementPacketsSent(uint64_t count, uint64_t bytes) {
    stats_.packetsSent += count;
    stats_.bytesSent += bytes;
}

void StatsCollector::incrementPacketsReceived(uint64_t count, uint64_t bytes) {
    stats_.packetsReceived += count;
    stats_.bytesReceived += bytes;
}

AttackStats StatsCollector::getCurrentStats() const {
    return stats_;
}

double StatsCollector::getDuration() const {
    if (!collecting_) return 0.0;
    
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(now - stats_.startTime).count();
}

double StatsCollector::getSendRate() const {
    return stats_.sendRate;
}

double StatsCollector::getReceiveRate() const {
    return stats_.receiveRate;
}

void StatsCollector::updateRates() {
    auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - lastUpdate_).count();
    
    if (elapsed > 0) {
        stats_.sendRate = stats_.packetsSent / elapsed;
        stats_.receiveRate = stats_.packetsReceived / elapsed;
    }
    
    lastUpdate_ = now;
    stats_.packetsSent = 0;
    stats_.packetsReceived = 0;
    stats_.bytesSent = 0;
    stats_.bytesReceived = 0;
}