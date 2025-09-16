#pragma once
#include <atomic>
#include <chrono>
#include <string>
#include <map>

struct AttackStats {
    std::atomic<uint64_t> packetsSent{0};
    std::atomic<uint64_t> packetsReceived{0};
    std::atomic<uint64_t> bytesSent{0};
    std::atomic<uint64_t> bytesReceived{0};
    std::atomic<double> sendRate{0.0}; // Packets per second
    std::atomic<double> receiveRate{0.0}; // Packets per second
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
};

class StatsCollector {
public:
    static StatsCollector& getInstance();
    
    void startCollection();
    void stopCollection();
    void reset();
    
    void incrementPacketsSent(uint64_t count = 1, uint64_t bytes = 0);
    void incrementPacketsReceived(uint64_t count = 1, uint64_t bytes = 0);
    
    AttackStats getCurrentStats() const;
    double getDuration() const;
    double getSendRate() const;
    double getReceiveRate() const;
    
    // 禁止拷贝和移动
    StatsCollector(const StatsCollector&) = delete;
    StatsCollector& operator=(const StatsCollector&) = delete;
    StatsCollector(StatsCollector&&) = delete;
    StatsCollector& operator=(StatsCollector&&) = delete;
    
private:
    StatsCollector();
    
    AttackStats stats_;
    std::chrono::steady_clock::time_point lastUpdate_;
    std::atomic<bool> collecting_{false};
    
    void updateRates();
};