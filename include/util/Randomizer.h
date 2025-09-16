#pragma once
#include <string>
#include <random>
#include <sstream>
#include <iomanip>

class Randomizer {
public:
    static Randomizer& getInstance();
    
    std::string randomIP();
    std::string randomMAC();
    int randomInt(int min, int max);
    std::string randomString(size_t length);
    
    // 禁止拷贝和移动
    Randomizer(const Randomizer&) = delete;
    Randomizer& operator=(const Randomizer&) = delete;
    Randomizer(Randomizer&&) = delete;
    Randomizer& operator=(Randomizer&&) = delete;
    
private:
    Randomizer();
    std::mt19937 rng_;
};