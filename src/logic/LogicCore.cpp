#include "logic/LogicCore.h"
#include "attack/AttackFactory.h"
#include "config/ConfigManager.h"
#include "util/Logger.h"

LogicCore& LogicCore::getInstance() {
    static LogicCore instance;
    return instance;
}

LogicCore::LogicCore() : running_(false) {}

LogicCore::~LogicCore() {
    stopAttack();
}

void LogicCore::startAttack() {
    if (running_) return;
    
    AttackConfig config = ConfigManager::getInstance().getAttackConfig();
    AttackType type = AttackFactory::stringToAttackType(config.type);
    
    if (type == AttackType::UNKNOWN) {
        Logger::getInstance().log(LogLevel::ERROR, "Unknown attack type: " + config.type);
        return;
    }
    
    try {
        currentAttack_ = AttackFactory::createAttack(type, config.target, config.interface);
        running_ = true;
        
        attackThread_ = std::make_unique<std::thread>([this]() {
            attackThread();
        });
        
        Logger::getInstance().log(LogLevel::INFO, "Attack started: " + config.type + " on " + config.target);
    } catch (const std::exception& e) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to start attack: " + std::string(e.what()));
        running_ = false;
    }
}

void LogicCore::stopAttack() {
    if (!running_) return;
    
    running_ = false;
    if (currentAttack_) {
        currentAttack_->stop();
    }
    
    if (attackThread_ && attackThread_->joinable()) {
        attackThread_->join();
    }
    
    attackThread_.reset();
    currentAttack_.reset();
    
    Logger::getInstance().log(LogLevel::INFO, "Attack stopped");
}

bool LogicCore::isAttackRunning() const {
    return running_;
}

void LogicCore::updateConfiguration() {
    // 重新加载配置
    ConfigManager::getInstance().loadConfig(ConfigManager::getInstance().getConfigPath());
}

void LogicCore::loadPreset(const std::string& presetName) {
    Preset preset = PresetManager::getInstance().getPreset(presetName);
    if (preset.name.empty()) {
        Logger::getInstance().log(LogLevel::ERROR, "Preset not found: " + presetName);
        return;
    }
    
    AttackConfig config;
    config.type = preset.attackType;
    config.target = preset.target;
    config.interface = preset.interface;
    config.duration = preset.duration;
    config.intensity = preset.intensity;
    
    ConfigManager::getInstance().setAttackConfig(config);
    Logger::getInstance().log(LogLevel::INFO, "Loaded preset: " + presetName);
}

void LogicCore::attackThread() {
    // 统计信息更新线程
    while (running_) {
        // 更新统计信息
        // 这里可以添加实际的统计信息收集逻辑
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}