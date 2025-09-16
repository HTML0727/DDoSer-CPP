#include "network/NetworkManager.h"
#include "util/Logger.h"
#include "util/Validator.h"
#include <ifaddrs.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

NetworkManager& NetworkManager::getInstance() {
    static NetworkManager instance;
    return instance;
}

std::vector<NetworkInterface> NetworkManager::getAvailableInterfaces() {
    std::vector<NetworkInterface> interfaces;
    struct ifaddrs *ifaddr, *ifa;
    
    if (getifaddrs(&ifaddr) == -1) {
        Logger::getInstance().log(LogLevel::ERROR, "getifaddrs failed");
        return interfaces;
    }
    
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        
        // 只处理IPv4接口
        if (ifa->ifa_addr->sa_family == AF_INET) {
            NetworkInterface ni;
            ni.name = ifa->ifa_name;
            ni.isUp = (ifa->ifa_flags & IFF_UP) != 0;
            
            // 获取IP地址
            char ip[INET_ADDRSTRLEN];
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN);
            ni.ipAddress = ip;
            
            // 获取网络掩码
            sa = (struct sockaddr_in *)ifa->ifa_netmask;
            inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN);
            ni.netmask = ip;
            
            // 获取MAC地址
            int fd = socket(AF_INET, SOCK_DGRAM, 0);
            if (fd >= 0) {
                struct ifreq ifr;
                memset(&ifr, 0, sizeof(ifr));
                strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ-1);
                
                if (ioctl(fd, SIOCGIFHWADDR, &ifr) >= 0) {
                    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
                    char macStr[18];
                    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
                            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                    ni.macAddress = macStr;
                }
                close(fd);
            }
            
            interfaces.push_back(ni);
        }
    }
    
    freeifaddrs(ifaddr);
    return interfaces;
}

bool NetworkManager::isValidIP(const std::string& ip) {
    return Validator::getInstance().isValidIP(ip);
}

bool NetworkManager::isValidMAC(const std::string& mac) {
    return Validator::getInstance().isValidMAC(mac);
}

std::string NetworkManager::getLocalIP() {
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    
    struct hostent* host = gethostbyname(hostname);
    if (host == nullptr) return "";
    
    return inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
}

std::string NetworkManager::getGatewayIP() {
    // 简化实现，实际应用中可能需要读取/proc/net/route或使用netlink
    return "192.168.1.1"; // 默认网关
}