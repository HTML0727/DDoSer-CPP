#include "config/ConfigManager.h"
#include "config/PresetManager.h"
#include "util/Logger.h"
#include "util/Helpers.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() : configJson_(nullptr) {
    const char* home = getenv("HOME");
    if (home) {
        configPath_ = std::string(home) + "/.ddoser/config.json";
    } else {
        configPath_ = "/etc/ddoser/config.json";
    }
    
    // 尝试加载配置
    if (!loadConfig(configPath_)) {
        // 创建默认配置
        configJson_ = cJSON_CreateObject();
        cJSON_AddStringToObject(configJson_, "version", "1.0");
        
        // 默认攻击配置
        cJSON* attackConfig = cJSON_CreateObject();
        cJSON_AddStringToObject(attackConfig, "type", "ICMP");
        cJSON_AddStringToObject(attackConfig, "target", "192.168.1.1");
        cJSON_AddStringToObject(attackConfig, "interface", "eth0");
        cJSON_AddNumberToObject(attackConfig, "duration", 60);
        cJSON_AddNumberToObject(attackConfig, "intensity", 10);
        cJSON_AddItemToObject(configJson_, "attack", attackConfig);
        
        // 目标列表
        cJSON* targets = cJSON_CreateArray();
        cJSON_AddItemToArray(targets, cJSON_CreateString("192.168.1.1"));
        cJSON_AddItemToObject(configJson_, "targets", targets);
        
        // 网络接口
        cJSON_AddStringToObject(configJson_, "interface", "eth0");
        
        // 保存默认配置
        saveConfig(configPath_);
    }
}

ConfigManager::~ConfigManager() {
    if (configJson_) {
        cJSON_Delete(configJson_);
    }
}

bool ConfigManager::loadConfig(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        Logger::getInstance().log(LogLevel::WARNING, "Config file not found: " + filePath);
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    configJson_ = cJSON_Parse(content.c_str());
    if (!configJson_) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to parse config file");
        return false;
    }
    
    configPath_ = filePath;
    return true;
}

bool ConfigManager::saveConfig(const std::string& filePath) {
    if (!configJson_) {
        Logger::getInstance().log(LogLevel::ERROR, "No config to save");
        return false;
    }
    
    char* jsonStr = cJSON_Print(configJson_);
    if (!jsonStr) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to serialize config");
        return false;
    }
    
    // 确保目录存在
    std::string dir = filePath.substr(0, filePath.find_last_of('/'));
    system(("mkdir -p " + dir).c_str());
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to open config file for writing");
        free(jsonStr);
        return false;
    }
    
    file << jsonStr;
    file.close();
    free(jsonStr);
    
    configPath_ = filePath;
    return true;
}

void ConfigManager::setAttackConfig(const AttackConfig& config) {
    if (!configJson_) return;
    
    cJSON* attackConfig = cJSON_GetObjectItemCaseSensitive(configJson_, "attack");
    if (!attackConfig) {
        attackConfig = cJSON_CreateObject();
        cJSON_AddItemToObject(configJson_, "attack", attackConfig);
    }
    
    cJSON_ReplaceItemInObject(attackConfig, "type", cJSON_CreateString(config.type.c_str()));
    cJSON_ReplaceItemInObject(attackConfig, "target", cJSON_CreateString(config.target.c_str()));
    cJSON_ReplaceItemInObject(attackConfig, "interface", cJSON_CreateString(config.interface.c_str()));
    cJSON_ReplaceItemInObject(attackConfig, "duration", cJSON_CreateNumber(config.duration));
    cJSON_ReplaceItemInObject(attackConfig, "intensity", cJSON_CreateNumber(config.intensity));
}

AttackConfig ConfigManager::getAttackConfig() const {
    AttackConfig config;
    
    if (!configJson_) return config;
    
    cJSON* attackConfig = cJSON_GetObjectItemCaseSensitive(configJson_, "attack");
    if (attackConfig) {
        cJSON* type = cJSON_GetObjectItemCaseSensitive(attackConfig, "type");
        cJSON* target = cJSON_GetObjectItemCaseSensitive(attackConfig, "target");
        cJSON* interface = cJSON_GetObjectItemCaseSensitive(attackConfig, "interface");
        cJSON* duration = cJSON_GetObjectItemCaseSensitive(attackConfig, "duration");
        cJSON* intensity = cJSON_GetObjectItemCaseSensitive(attackConfig, "intensity");
        
        if (type) config.type = type->valuestring;
        if (target) config.target = target->valuestring;
        if (interface) config.interface = interface->valuestring;
        if (duration) config.duration = duration->valueint;
        if (intensity) config.intensity = intensity->valueint;
    }
    
    return config;
}

void ConfigManager::addTarget(const std::string& target) {
    if (!configJson_) return;
    
    cJSON* targets = cJSON_GetObjectItemCaseSensitive(configJson_, "targets");
    if (!targets) {
        targets = cJSON_CreateArray();
        cJSON_AddItemToObject(configJson_, "targets", targets);
    }
    
    // 检查是否已存在
    cJSON* targetItem = nullptr;
    cJSON_ArrayForEach(targetItem, targets) {
        if (targetItem->valuestring && std::string(targetItem->valuestring) == target) {
            return; // 已存在
        }
    }
    
    cJSON_AddItemToArray(targets, cJSON_CreateString(target.c_str()));
}

void ConfigManager::removeTarget(const std::string& target) {
    if (!configJson_) return;
    
    cJSON* targets = cJSON_GetObjectItemCaseSensitive(configJson_, "targets");
    if (!targets) return;
    
    int index = 0;
    cJSON* targetItem = nullptr;
    cJSON_ArrayForEach(targetItem, targets) {
        if (targetItem->valuestring && std::string(targetItem->valuestring) == target) {
            cJSON_DeleteItemFromArray(targets, index);
            return;
        }
        index++;
    }
}

std::vector<std::string> ConfigManager::getTargets() const {
    std::vector<std::string> targets;
    
    if (!configJson_) return targets;
    
    cJSON* targetsArray = cJSON_GetObjectItemCaseSensitive(configJson_, "targets");
    if (targetsArray && cJSON_IsArray(targetsArray)) {
        cJSON* targetItem = nullptr;
        cJSON_ArrayForEach(targetItem, targetsArray) {
            if (targetItem->valuestring) {
                targets.push_back(targetItem->valuestring);
            }
        }
    }
    
    return targets;
}

void ConfigManager::setNetworkInterface(const std::string& interface) {
    if (!configJson_) return;
    
    cJSON_ReplaceItemInObject(configJson_, "interface", cJSON_CreateString(interface.c_str()));
}

std::string ConfigManager::getNetworkInterface() const {
    if (!configJson_) return "";
    
    cJSON* interface = cJSON_GetObjectItemCaseSensitive(configJson_, "interface");
    if (interface && interface->valuestring) {
        return interface->valuestring;
    }
    
    return "";
}

bool ConfigManager::loadPreset(const std::string& presetName) {
    // 实现预设加载逻辑
    return PresetManager::getInstance().getPreset(presetName).name == presetName;
}

bool ConfigManager::savePreset(const std::string& presetName) {
    // 实现预设保存逻辑
    AttackConfig config = getAttackConfig();
    Preset preset;
    preset.name = presetName;
    preset.attackType = config.type;
    preset.target = config.target;
    preset.interface = config.interface;
    preset.duration = config.duration;
    preset.intensity = config.intensity;
    
    return PresetManager::getInstance().savePreset(preset);
}

std::vector<std::string> ConfigManager::getAvailablePresets() {
    std::vector<std::string> presets;
    for (const auto& preset : PresetManager::getInstance().getPresets()) {
        presets.push_back(preset.name);
    }
    return presets;
}