#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "cJSON.h"

struct AttackConfig {
    std::string type;
    std::string target;
    std::string interface;
    int duration;
    int intensity;
};

class ConfigManager {
public:
    static ConfigManager& getInstance();
    
    bool loadConfig(const std::string& filePath);
    bool saveConfig(const std::string& filePath);
    
    void setAttackConfig(const AttackConfig& config);
    AttackConfig getAttackConfig() const;
    
    void addTarget(const std::string& target);
    void removeTarget(const std::string& target);
    std::vector<std::string> getTargets() const;
    
    void setNetworkInterface(const std::string& interface);
    std::string getNetworkInterface() const;
    
    std::string getConfigPath() const { return configPath_; }
    
    // 预设管理
    bool loadPreset(const std::string& presetName);
    bool savePreset(const std::string& presetName);
    std::vector<std::string> getAvailablePresets();
    
    // 禁止拷贝和移动
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;
    
private:
    ConfigManager();
    ~ConfigManager();
    
    cJSON* configJson_;
    std::string configPath_;
};