#pragma once
#include <string>

class Validator {
public:
    static Validator& getInstance();
    
    bool isValidIP(const std::string& ip);
    bool isValidPort(int port);
    bool isValidMAC(const std::string& mac);
    bool isValidDomain(const std::string& domain);
    bool isPrivateIP(const std::string& ip);
    bool isLocalhost(const std::string& ip);
    
    // 禁止拷贝和移动
    Validator(const Validator&) = delete;
    Validator& operator=(const Validator&) = delete;
    Validator(Validator&&) = delete;
    Validator& operator=(Validator&&) = delete;
    
private:
    Validator() = default;
};