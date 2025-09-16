#include "attack/AttackFactory.h"
#include "attack/ARPAttack.h"
#include "attack/ICMPAttack.h"
#include "attack/UDPAttack.h"
#include "attack/TCPAttack.h"
#include "attack/DNSAttack.h"
#include <stdexcept>
#include <algorithm>
#include "util/Logger.h"

std::shared_ptr<AttackBase> AttackFactory::createAttack(
    AttackType type, 
    const std::string& target, 
    const std::string& interface) {
    
    switch (type) {
        case AttackType::ARP:
            return std::make_shared<ARPAttack>(target, interface);
        case AttackType::ICMP:
            return std::make_shared<ICMPAttack>(target, 64, 100);
        case AttackType::UDP:
            return std::make_shared<UDPAttack>(target, 80, 64, 100);
        case AttackType::TCP:
            return std::make_shared<TCPAttack>(target, 80, 64, 100);
        case AttackType::DNS:
            return std::make_shared<DNSAttack>(target, "example.com", 1);
        default:
            Logger::getInstance().log(LogLevel::ERROR, "Unsupported attack type");
            throw std::runtime_error("Unsupported attack type");
    }
}

std::vector<std::string> AttackFactory::getAvailableAttacks() {
    return {
        "ARP",
        "PPPoE",
        "ICMP",
        "TCP",
        "UDP",
        "DNS",
        "DHCP",
        "HSRP"
    };
}

AttackType AttackFactory::stringToAttackType(const std::string& typeStr) {
    std::string upper = typeStr;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    if (upper == "ARP") return AttackType::ARP;
    if (upper == "PPPOE") return AttackType::PPPoE;
    if (upper == "ICMP") return AttackType::ICMP;
    if (upper == "TCP") return AttackType::TCP;
    if (upper == "UDP") return AttackType::UDP;
    if (upper == "DNS") return AttackType::DNS;
    if (upper == "DHCP") return AttackType::DHCP;
    if (upper == "HSRP") return AttackType::HSRP;
    
    return AttackType::UNKNOWN;
}

std::string AttackFactory::attackTypeToString(AttackType type) {
    switch (type) {
        case AttackType::ARP: return "ARP";
        case AttackType::PPPoE: return "PPPoE";
        case AttackType::ICMP: return "ICMP";
        case AttackType::TCP: return "TCP";
        case AttackType::UDP: return "UDP";
        case AttackType::DNS: return "DNS";
        case AttackType::DHCP: return "DHCP";
        case AttackType::HSRP: return "HSRP";
        default: return "UNKNOWN";
    }
}