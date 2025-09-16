#include "config/PresetManager.h"
#include "util/Logger.h"
#include "util/Helpers.h"
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <cstdlib>

PresetManager& PresetManager::getInstance() {
    static PresetManager instance;
    return instance;
}

PresetManager::PresetManager() {
    const char* home = getenv("HOME");
    if (home) {
        presetsDirectory_ = std::string(home) + "/.ddoser/presets";
    } else {
        presetsDirectory_ = "/etc/ddoser/presets";
    }
    loadPresets(presetsDirectory_);
}

bool PresetManager::loadPresets(const std::string& directoryPath) {
    presetsDirectory_ = directoryPath;
    presets_.clear();
    
    // 确保目录存在
    system(("mkdir -p " + directoryPath).c_str());
    
    DIR* dir = opendir(directoryPath.c_str());
    if (!dir) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to open presets directory");
        return false;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) {
            std::string filename = entry->d_name;
            if (filename.size() > 5 && filename.substr(filename.size() - 5) == ".json") {
                std::string presetName = filename.substr(0, filename.size() - 5);
                std::string filePath = directoryPath + "/" + filename;
                
                std::ifstream file(filePath);
                if (file.is_open()) {
                    std::stringstream buffer;
                    buffer << file.rdbuf();
                    std::string content = buffer.str();
                    
                    cJSON* json = cJSON_Parse(content.c_str());
                    if (json) {
                        Preset preset = parsePresetFromJSON(json);
                        presets_[presetName] = preset;
                        cJSON_Delete(json);
                    }
                }
            }
        }
    }
    
    closedir(dir);
    return true;
}

bool PresetManager::savePreset(const Preset& preset) {
    cJSON* json = createJSONFromPreset(preset);
    if (!json) return false;
    
    char* jsonStr = cJSON_Print(json);
    cJSON_Delete(json);
    
    if (!jsonStr) return false;
    
    std::string filePath = presetsDirectory_ + "/" + preset.name + ".json";
    std::ofstream file(filePath);
    if (!file.is_open()) {
        free(jsonStr);
        return false;
    }
    
    file << jsonStr;
    file.close();
    free(jsonStr);
    
    // 更新内存中的预设
    presets_[preset.name] = preset;
    return true;
}

bool PresetManager::deletePreset(const std::string& name) {
    std::string filePath = presetsDirectory_ + "/" + name + ".json";
    if (remove(filePath.c_str()) == 0) {
        presets_.erase(name);
        return true;
    }
    return false;
}

std::vector<Preset> PresetManager::getPresets() const {
    std::vector<Preset> result;
    for (const auto& pair : presets_) {
        result.push_back(pair.second);
    }
    return result;
}

Preset PresetManager::getPreset(const std::string& name) const {
    auto it = presets_.find(name);
    if (it != presets_.end()) {
        return it->second;
    }
    return Preset();
}

bool PresetManager::presetExists(const std::string& name) const {
    return presets_.find(name) != presets_.end();
}

Preset PresetManager::parsePresetFromJSON(cJSON* json) const {
    Preset preset;
    
    cJSON* name = cJSON_GetObjectItemCaseSensitive(json, "name");
    cJSON* description = cJSON_GetObjectItemCaseSensitive(json, "description");
    cJSON* attackType = cJSON_GetObjectItemCaseSensitive(json, "attackType");
    cJSON* target = cJSON_GetObjectItemCaseSensitive(json, "target");
    cJSON* interface = cJSON_GetObjectItemCaseSensitive(json, "interface");
    cJSON* duration = cJSON_GetObjectItemCaseSensitive(json, "duration");
    cJSON* intensity = cJSON_GetObjectItemCaseSensitive(json, "intensity");
    
    if (name) preset.name = name->valuestring;
    if (description) preset.description = description->valuestring;
    if (attackType) preset.attackType = attackType->valuestring;
    if (target) preset.target = target->valuestring;
    if (interface) preset.interface = interface->valuestring;
    if (duration) preset.duration = duration->valueint;
    if (intensity) preset.intensity = intensity->valueint;
    
    return preset;
}

cJSON* PresetManager::createJSONFromPreset(const Preset& preset) const {
    cJSON* json = cJSON_CreateObject();
    
    cJSON_AddStringToObject(json, "name", preset.name.c_str());
    cJSON_AddStringToObject(json, "description", preset.description.c_str());
    cJSON_AddStringToObject(json, "attackType", preset.attackType.c_str());
    cJSON_AddStringToObject(json, "target", preset.target.c_str());
    cJSON_AddStringToObject(json, "interface", preset.interface.c_str());
    cJSON_AddNumberToObject(json, "duration", preset.duration);
    cJSON_AddNumberToObject(json, "intensity", preset.intensity);
    
    return json;
}