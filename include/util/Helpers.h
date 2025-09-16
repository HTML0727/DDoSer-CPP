#pragma once
#include <string>
#include <vector>

namespace helpers {
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    std::string trim(const std::string& str);
    bool startsWith(const std::string& str, const std::string& prefix);
    bool endsWith(const std::string& str, const std::string& suffix);
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);
    bool isRoot();
    void daemonize();
}