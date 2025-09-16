#pragma once
#include "AttackBase.h"
#include <string>

class UDPAttack : public AttackBase {
public:
    UDPAttack(const std::string& targetIP, int targetPort, int packetSize, int delayMs);
    ~UDPAttack() override;
    
    void start() override;
    void stop() override;
    std::string getName() const override { return "UDP Flood"; }
    std::string getDescription() const override;
    
    void setPacketSize(int size) { packetSize_ = size; }
    void setDelay(int delayMs) { delayMs_ = delayMs; }
    void setPort(int port) { targetPort_ = port; }
    
private:
    std::string targetIP_;
    int targetPort_;
    int packetSize_;
    int delayMs_;
};