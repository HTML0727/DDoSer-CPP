#pragma once
#include <vector>
#include <cstdint>
#include <string>

class PacketBuilder {
public:
    static std::vector<uint8_t> buildARPPacket(
        const std::string& srcMac, 
        const std::string& srcIp,
        const std::string& dstMac, 
        const std::string& dstIp,
        bool isRequest = true
    );
    
    static std::vector<uint8_t> buildICMPPacket(
        const std::string& srcIp,
        const std::string& dstIp,
        uint16_t id, 
        uint16_t seq,
        const std::string& payload = ""
    );
    
    static std::vector<uint8_t> buildUDPPacket(
        const std::string& srcIp,
        const std::string& dstIp,
        uint16_t srcPort,
        uint16_t dstPort,
        const std::string& payload = ""
    );
    
    static std::vector<uint8_t> buildTCPPacket(
        const std::string& srcIp,
        const std::string& dstIp,
        uint16_t srcPort,
        uint16_t dstPort,
        uint32_t seqNum,
        uint32_t ackNum,
        uint8_t flags,
        const std::string& payload = ""
    );
};