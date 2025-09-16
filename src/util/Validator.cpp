#include "util/Validator.h"
#include <regex>
#include <arpa/inet.h>

Validator& Validator::getInstance() {
    static Validator instance;
    return instance;
}

bool Validator::isValidIP(const std::string& ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) != 0;
}

bool Validator::isValidPort(int port) {
    return port > 0 && port <= 65535;
}

bool Validator::isValidMAC(const std::string& mac) {
    std::regex pattern("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");
    return std::regex_match(mac, pattern);
}

bool Validator::isValidDomain(const std::string& domain) {
    if (domain.length() > 253) return false;
    
    std::regex pattern("^([a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?\\.)+[a-zA-Z]{2,}$");
    return std::regex_match(domain, pattern);
}

bool Validator::isPrivateIP(const std::string& ip) {
    if (!isValidIP(ip)) return false;
    
    struct in_addr addr;
    inet_pton(AF_INET, ip.c_str(), &addr);
    uint32_t ip_num = ntohl(addr.s_addr);
    
    // 10.0.0.0/8
    if ((ip_num & 0xFF000000) == 0x0A000000) return true;
    
    // 172.16.0.0/12
    if ((ip_num & 0xFFF00000) == 0xAC100000) return true;
    
    // 192.168.0.0/16
    if ((ip_num & 0xFFFF0000) == 0xC0A80000) return true;
    
    // 127.0.0.0/8
    if ((ip_num & 0xFF000000) == 0x7F000000) return true;
    
    return false;
}

bool Validator::isLocalhost(const std::string& ip) {
    return ip == "127.0.0.1" || ip == "::1" || ip == "localhost";
}