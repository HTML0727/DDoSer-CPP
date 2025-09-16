#pragma once
#include <string>
#include <vector>
#include <map>
#include "cJSON.h"

struct Preset {
    std::string name;
    std::string description;
    std::string attackType;
    std::string target;
    std::string interface;
    int duration;
    int intensity;
};

class PresetManager {
public:
    static PresetManager& getInstance();
    
    bool loadPresets(const std::string& directoryPath);
    bool savePreset(const Preset& preset);
    bool deletePreset(const std::string& name);
    
    std::vector<Preset> getPresets() const;
    Preset getPreset(const std::string& name) const;
    bool presetExists(const std::string& name) const;
    
    // 禁止拷贝和移动
    PresetManager(const PresetManager&) = delete;
    PresetManager& operator=(const PresetManager&) = delete;
    PresetManager(PresetManager&&) = delete;
    PresetManager& operator=(PresetManager&&) = delete;
    
private:
    PresetManager();
    
    std::map<std::string, Preset> presets_;
    std::string presetsDirectory_;
    
    Preset parsePresetFromJSON(cJSON* json) const;
    cJSON* createJSONFromPreset(const Preset& preset) const;
};