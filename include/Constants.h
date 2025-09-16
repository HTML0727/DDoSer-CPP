#pragma once
#include <string>

namespace Constants {
    // 应用信息
    const std::string APP_NAME = "DDoSer";
    const std::string APP_VERSION = "1.0.0";
    const std::string APP_DESCRIPTION = "DDoS Attack Tool for Educational Purposes";
    
    // 默认配置
    const std::string DEFAULT_CONFIG_PATH = "/etc/ddoser/config.json";
    const std::string DEFAULT_PRESETS_DIR = "/etc/ddoser/presets";
    const std::string DEFAULT_LOG_PATH = "/var/log/ddoser.log";
    
    // UI 常量
    const int MAIN_WINDOW_HEIGHT = 24;
    const int MAIN_WINDOW_WIDTH = 80;
    const int STATUS_WINDOW_HEIGHT = 4;
    
    // 攻击参数默认值
    const int DEFAULT_PACKET_SIZE = 64;
    const int DEFAULT_DELAY_MS = 100;
    const int DEFAULT_DURATION = 60; // seconds
    const int DEFAULT_INTENSITY = 10; // threads
    
    // 颜色对
    namespace ColorPairs {
        const int DEFAULT = 1;
        const int HIGHLIGHT = 2;
        const int WARNING = 3;
        const int ERROR = 4;
        const int SUCCESS = 5;
    }
    
    // 键盘快捷键
    namespace KeyBindings {
        const int RUN = 'r';
        const int ADD_TARGET = KEY_F(1);
        const int NETWORK_CONFIG = KEY_F(2);
        const int VIEW_STATS = KEY_F(3);
        const int SAVE_CONFIG = KEY_F(4);
        const int LOAD_PRESET = KEY_F(5);
        const int QUIT = 'q';
        const int HELP = '?';
    }
}