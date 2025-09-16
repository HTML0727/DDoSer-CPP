#include "network/PacketBuilder.h"
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <cstring>
#include <stdexcept>

std::vector<uint8_t> PacketBuilder::buildARPPacket(
    const std::string& srcMac, 
    const std::string& srcIp,
    const std::string& dstMac, 
    const std::string& dstIp,
    bool isRequest) {
    
    std::vector<uint8_t> packet(sizeof(struct ether_header) + sizeof(struct arphdr) + 20);
    uint8_t* ptr = packet.data();
    
    // 以太网头部
    struct ether_header* eth = (struct ether_header*)ptr;
    
    // 解析MAC地址
    sscanf(srcMac.c_str(), "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx",
          &eth->ether_shost[0], &eth->ether_shost[1], &eth->ether_shost[2],
          &eth->ether_shost[3], &eth->ether_shost[4], &eth->ether_shost[5]);
    
    sscanf(dstMac.c_str(), "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx",
          &eth->ether_dhost[0], &eth->ether_dhost[1], &eth->ether_dhost[2],
          &eth->ether_dhost[3], &eth->ether_dhost[4], &eth->ether_dhost[5]);
    
    eth->ether_type = htons(ETHERTYPE_ARP);
    ptr += sizeof(struct ether_header);
    
    // ARP头部
    struct arphdr* arp = (struct arphdr*)ptr;
    arp->ar_hrd = htons(ARPHRD_ETHER);
    arp->ar_pro = htons(ETHERTYPE_IP);
    arp->ar_hln = 6;
    arp->ar_pln = 4;
    arp->ar_op = htons(isRequest ? ARPOP_REQUEST : ARPOP_REPLY);
    ptr += sizeof(struct arphdr);
    
    // 发送方MAC
    sscanf(srcMac.c_str(), "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx",
          ptr, ptr+1, ptr+2, ptr+3, ptr+4, ptr+5);
    ptr += 6;
    
    // 发送方IP
    inet_pton(AF_INET, srcIp.c_str(), ptr);
    ptr += 4;
    
    // 目标MAC
    if (isRequest) {
        // 请求包中目标MAC设为广播地址
        memset(ptr, 0xFF, 6);
    } else {
        sscanf(dstMac.c_str(), "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx",
              ptr, ptr+1, ptr+2, ptr+3, ptr+4, ptr+5);
    }
    ptr += 6;
    
    // 目标IP
    inet_pton(AF_INET, dstIp.c_str(), ptr);
    
    return packet;
}

std::vector<uint8_t> PacketBuilder::buildICMPPacket(
    const std::string& srcIp,
    const std::string& dstIp,
    uint16_t id, 
    uint16_t seq,
    const std::string& payload) {
    
    size_t payloadSize = payload.size();
    size_t packetSize = sizeof(struct ip) + sizeof(struct icmphdr) + payloadSize;
    std::vector<uint8_t> packet(packetSize);
    uint8_t* ptr = packet.data();
    
    // IP头部
    struct ip* iph = (struct ip*)ptr;
    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    iph->ip_len = htons(packetSize);
    iph->ip_id = htons(54321);
    iph->ip_off = 0;
    iph->ip_ttl = 255;
    iph->ip_p = IPPROTO_ICMP;
    iph->ip_sum = 0;
    inet_pton(AF_INET, srcIp.c_str(), &iph->ip_src);
    inet_pton(AF_INET, dstIp.c_str(), &iph->ip_dst);
    iph->ip_sum = 0; // 计算校验和
    
    ptr += sizeof(struct ip);
    
    // ICMP头部
    struct icmphdr* icmph = (struct icmphdr*)ptr;
    icmph->type = ICMP_ECHO;
    icmph->code = 0;
    icmph->un.echo.id = htons(id);
    icmph->un.echo.sequence = htons(seq);
    icmph->checksum = 0;
    
    ptr += sizeof(struct icmphdr);
    
    // 载荷
    if (!payload.empty()) {
        memcpy(ptr, payload.data(), payloadSize);
    }
    
    // 计算ICMP校验和
    icmph->checksum = 0;
    uint32_t sum = 0;
    uint16_t* words = (uint16_t*)icmph;
    size_t len = sizeof(struct icmphdr) + payloadSize;
    
    while (len > 1) {
        sum += *words++;
        len -= 2;
    }
    
    if (len > 0) {
        sum += *(uint8_t*)words;
    }
    
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    icmph->checksum = ~sum;
    
    return packet;
}

std::vector<uint8_t> PacketBuilder::buildUDPPacket(
    const std::string& srcIp,
    const std::string& dstIp,
    uint16_t srcPort,
    uint16_t dstPort,
    const std::string& payload) {
    
    size_t payloadSize = payload.size();
    size_t packetSize = sizeof(struct ip) + sizeof(struct udphdr) + payloadSize;
    std::vector<uint8_t> packet(packetSize);
    uint8_t* ptr = packet.data();
    
    // IP头部
    struct ip* iph = (struct ip*)ptr;
    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    iph->ip_len = htons(packetSize);
    iph->ip_id = htons(54321);
    iph->ip_off = 0;
    iph->ip_ttl = 255;
    iph->ip_p = IPPROTO_UDP;
    iph->ip_sum = 0;
    inet_pton(AF_INET, srcIp.c_str(), &iph->ip_src);
    inet_pton(AF_INET, dstIp.c_str(), &iph->ip_dst);
    iph->ip_sum = 0; // 计算校验和
    
    ptr += sizeof(struct ip);
    
    // UDP头部
    struct udphdr* udph = (struct udphdr*)ptr;
    udph->source = htons(srcPort);
    udph->dest = htons(dstPort);
    udph->len = htons(sizeof(struct udphdr) + payloadSize);
    udph->check = 0;
    
    ptr += sizeof(struct udphdr);
    
    // 载荷
    if (!payload.empty()) {
        memcpy(ptr, payload.data(), payloadSize);
    }
    
    return packet;
}

std::vector<uint8_t> PacketBuilder::buildTCPPacket(
    const std::string& srcIp,
    const std::string& dstIp,
    uint16_t srcPort,
    uint16_t dstPort,
    uint32_t seqNum,
    uint32_t ackNum,
    uint8_t flags,
    const std::string& payload) {
    
    size_t payloadSize = payload.size();
    size_t packetSize = sizeof(struct ip) + sizeof(struct tcphdr) + payloadSize;
    std::vector<uint8_t> packet(packetSize);
    uint8_t* ptr = packet.data();
    
    // IP头部
    struct ip* iph = (struct ip*)ptr;
    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    iph->ip_len = htons(packetSize);
    iph->ip_id = htons(54321);
    iph->ip_off = 0;
    iph->ip_ttl = 255;
    iph->ip_p = IPPROTO_TCP;
    iph->ip_sum = 0;
    inet_pton(AF_INET, srcIp.c_str(), &iph->ip_src);
    inet_pton(AF_INET, dstIp.c_str(), &iph->ip_dst);
    iph->ip_sum = 0; // 计算校验和
    
    ptr += sizeof(struct ip);
    
    // TCP头部
    struct tcphdr* tcph = (struct tcphdr*)ptr;
    tcph->source = htons(srcPort);
    tcph->dest = htons(dstPort);
    tcph->seq = htonl(seqNum);
    tcph->ack_seq = htonl(ackNum);
    tcph->doff = 5;
    tcph->fin = (flags & 0x01) ? 1 : 0;
    tcph->syn = (flags & 0x02) ? 1 : 0;
    tcph->rst = (flags & 0x04) ? 1 : 0;
    tcph->psh = (flags & 0x08) ? 1 : 0;
    tcph->ack = (flags & 0x10) ? 1 : 0;
    tcph->urg = (flags & 0x20) ? 1 : 0;
    tcph->window = htons(5840);
    tcph->check = 0;
    tcph->urg_ptr = 0;
    
    ptr += sizeof(struct tcphdr);
    
    // 载荷
    if (!payload.empty()) {
        memcpy(ptr, payload.data(), payloadSize);
    }
    
    return packet;
}