#pragma once
#include "AttackBase.h"
#include <string>

class ICMPAttack : public AttackBase {
public:
    ICMPAttack(const std::string& targetIP, int packetSize, int delayMs);
    ~ICMPAttack() override;
    
    void start() override;
    void stop() override;
    std::string getName() const override { return "ICMP Flood"; }
    std::string getDescription() const override;
    
    void setPacketSize(int size) { packetSize_ = size; }
    void setDelay(int delayMs) { delayMs_ = delayMs; }
    
private:
    std::string targetIP_;
    int packetSize_;
    int delayMs_;
};