#pragma once
#include "AttackBase.h"
#include <string>

class DNSAttack : public AttackBase {
public:
    DNSAttack(const std::string& targetDNS, const std::string& domain, int queryType);
    ~DNSAttack() override;
    
    void start() override;
    void stop() override;
    std::string getName() const override { return "DNS Amplification"; }
    std::string getDescription() const override;
    
    void setDomain(const std::string& domain) { domain_ = domain; }
    void setQueryType(int type) { queryType_ = type; }
    
private:
    std::string targetDNS_;
    std::string domain_;
    int queryType_;
    
    std::string buildDNSQuery(const std::string& domain, int queryType);
};