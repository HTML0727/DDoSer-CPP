#pragma once
#include "AttackBase.h"
#include <string>

class ARPAttack : public AttackBase {
public:
    ARPAttack(const std::string& targetIP, const std::string& interface);
    ~ARPAttack() override;

    void start() override;
    void stop() override;
    std::string getName() const override { return "ARP Attack"; }
    std::string getDescription() const override;

private:
    std::string targetIP_;
    std::string interface_;
};