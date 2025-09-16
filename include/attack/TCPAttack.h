#pragma once
#include "AttackBase.h"
#include <string>

class TCPAttack : public AttackBase {
public:
    TCPAttack(const std::string& targetIP, int targetPort, int packetSize, int delayMs);
    ~TCPAttack() override;
    
    void start() override;
    void stop() override;
    std::string getName() const override { return "TCP Flood"; }
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