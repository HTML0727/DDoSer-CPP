#include "util/Randomizer.h"
#include <random>
#include <sstream>
#include <iomanip>

Randomizer& Randomizer::getInstance() {
    static Randomizer instance;
    return instance;
}

Randomizer::Randomizer() : rng_(std::random_device{}()) {}

std::string Randomizer::randomIP() {
    std::uniform_int_distribution<int> dist(1, 254);
    std::stringstream ss;
    ss << dist(rng_) << "." << dist(rng_) << "." 
       << dist(rng_) << "." << dist(rng_);
    return ss.str();
}

std::string Randomizer::randomMAC() {
    std::uniform_int_distribution<int> dist(0, 255);
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 6; ++i) {
        if (i > 0) ss << ":";
        ss << std::setw(2) << dist(rng_);
    }
    return ss.str();
}

int Randomizer::randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng_);
}

std::string Randomizer::randomString(size_t length) {
    static const char chars[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    std::uniform_int_distribution<int> dist(0, sizeof(chars) - 2);
    std::string result;
    result.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        result += chars[dist(rng_)];
    }
    
    return result;
}