#pragma once
#include <memory>
#include <atomic>
#include <thread>
#include <vector>
#include "attack/AttackBase.h"

class LogicCore {
public:
    static LogicCore& getInstance();
    
    void startAttack();
    void stopAttack();
    bool isAttackRunning() const;
    
    void updateConfiguration();
    void loadPreset(const std::string& presetName);
    
    // 统计信息
    int getPacketsSent() const { return packetsSent_; }
    int getPacketsReceived() const { return packetsReceived_; }
    double getAttackRate() const { return attackRate_; }
    
    // 禁止拷贝和移动
    LogicCore(const LogicCore&) = delete;
    LogicCore& operator=(const LogicCore&) = delete;
    LogicCore(LogicCore&&) = delete;
    LogicCore& operator=(LogicCore&&) = delete;
    
private:
    LogicCore();
    ~LogicCore();
    
    void attackThread();
    
    std::atomic<bool> running_{false};
    std::unique_ptr<std::thread> attackThread_;
    std::shared_ptr<AttackBase> currentAttack_;
    
    // 统计信息
    std::atomic<int> packetsSent_{0};
    std::atomic<int> packetsReceived_{0};
    std::atomic<double> attackRate_{0.0};
};