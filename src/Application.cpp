#include "Application.h"
#include "ui/UIManager.h"
#include "config/ConfigManager.h"
#include "Constants.h"
#include "util/Logger.h"
#include "util/Helpers.h"
#include <iostream>
#include <cstring>

Application* Application::instance_ = nullptr;

Application& Application::getInstance() {
    if (!instance_) {
        throw std::runtime_error("Application not initialized");
    }
    return *instance_;
}

Application::Application(int argc, char* argv[]) 
    : daemonMode_(false), running_(false) {
    if (instance_) {
        throw std::runtime_error("Application already initialized");
    }
    instance_ = this;
    
    parseArguments(argc, argv);
}

Application::~Application() {
    cleanup();
    instance_ = nullptr;
}

int Application::run() {
    if (running_) return 0;
    
    if (!initialize()) {
        return 1;
    }
    
    running_ = true;
    
    if (isDaemon()) {
        helpers::daemonize();
    }
    
    try {
        UIManager::getInstance().run();
    } catch (const std::exception& e) {
        Logger::getInstance().log(LogLevel::ERROR, "Application error: " + std::string(e.what()));
        return 1;
    }
    
    return 0;
}

void Application::stop() {
    running_ = false;
}

bool Application::initialize() {
    // 检查root权限
    if (!helpers::isRoot()) {
        std::cerr << "Error: This program must be run as root" << std::endl;
        return false;
    }
    
    // 初始化日志
    Logger::getInstance().setLogFile(getArgument("log", Constants::DEFAULT_LOG_PATH));
    
    // 初始化UI
    try {
        UIManager::getInstance().initialize();
    } catch (const std::exception& e) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to initialize UI: " + std::string(e.what()));
        return false;
    }
    
    // 加载配置
    std::string configPath = getArgument("config", Constants::DEFAULT_CONFIG_PATH);
    if (!ConfigManager::getInstance().loadConfig(configPath)) {
        Logger::getInstance().log(LogLevel::WARNING, "Using default configuration");
    }
    
    Logger::getInstance().log(LogLevel::INFO, "Application initialized");
    return true;
}

void Application::cleanup() {
    UIManager::getInstance().cleanup();
    Logger::getInstance().log(LogLevel::INFO, "Application cleaned up");
}

void Application::parseArguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-d" || arg == "--daemon") {
            daemonMode_ = true;
        } else if (arg == "-c" || arg == "--config") {
            if (i + 1 < argc) {
                arguments_["config"] = argv[++i];
            }
        } else if (arg == "-l" || arg == "--log") {
            if (i + 1 < argc) {
                arguments_["log"] = argv[++i];
            }
        } else if (arg == "-h" || arg == "--help") {
            showHelp();
            exit(0);
        } else if (arg == "-v" || arg == "--version") {
            std::cout << Constants::APP_NAME << " v" << Constants::APP_VERSION << std::endl;
            exit(0);
        }
    }
}

std::string Application::getArgument(const std::string& name, const std::string& defaultValue) const {
    auto it = arguments_.find(name);
    if (it != arguments_.end()) {
        return it->second;
    }
    return defaultValue;
}

bool Application::hasArgument(const std::string& name) const {
    return arguments_.find(name) != arguments_.end();
}

void Application::showHelp() const {
    std::cout << "Usage: " << Constants::APP_NAME << " [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -d, --daemon         Run as daemon" << std::endl;
    std::cout << "  -c, --config FILE    Use specified config file" << std::endl;
    std::cout << "  -l, --log FILE       Use specified log file" << std::endl;
    std::cout << "  -h, --help           Show this help message" << std::endl;
    std::cout << "  -v, --version        Show version information" << std::endl;
}