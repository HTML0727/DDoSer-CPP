#pragma once
#include <string>
#include <atomic>
#include <memory>
#include <vector>

class AttackBase {
public:
    virtual ~AttackBase() = default;
    
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    
    bool isRunning() const { return running_; }
    
protected:
    std::atomic<bool> running_{false};
};

// 攻击类型枚举
enum class AttackType {
    ARP,
    PPPoE,
    ICMP,
    TCP,
    UDP,
    DNS,
    DHCP,
    HSRP,
    UNKNOWN
};