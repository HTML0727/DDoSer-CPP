#pragma once
#include <string>
#include <vector>

struct NetworkInterface {
    std::string name;
    std::string ipAddress;
    std::string macAddress;
    std::string netmask;
    bool isUp;
};

class NetworkManager {
public:
    static NetworkManager& getInstance();
    
    std::vector<NetworkInterface> getAvailableInterfaces();
    bool isValidIP(const std::string& ip);
    bool isValidMAC(const std::string& mac);
    std::string getLocalIP();
    std::string getGatewayIP();
    
    // 禁止拷贝和移动
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;
    NetworkManager(NetworkManager&&) = delete;
    NetworkManager& operator=(NetworkManager&&) = delete;
    
private:
    NetworkManager() = default;
};