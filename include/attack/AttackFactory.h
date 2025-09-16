#pragma once
#include "AttackBase.h"
#include <memory>
#include <string>
#include <map>

class AttackFactory {
public:
    static std::shared_ptr<AttackBase> createAttack(
        AttackType type, 
        const std::string& target, 
        const std::string& interface = ""
    );
    
    static std::vector<std::string> getAvailableAttacks();
    static AttackType stringToAttackType(const std::string& typeStr);
    static std::string attackTypeToString(AttackType type);
};