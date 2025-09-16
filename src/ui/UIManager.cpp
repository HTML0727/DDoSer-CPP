#include "ui/UIManager.h"
#include "ui/InputHandler.h"
#include "ui/ThemeManager.h"
#include "logic/LogicCore.h"
#include "config/ConfigManager.h"
#include "Constants.h"
#include "util/Logger.h"
#include <stdexcept>

UIManager& UIManager::getInstance() {
    static UIManager instance;
    return instance;
}

UIManager::UIManager() 
    : mainWindow_(nullptr), statusWindow_(nullptr), inputWindow_(nullptr),
      initialized_(false), currentPage_(0) {}

UIManager::~UIManager() {
    cleanup();
}

void UIManager::initialize() {
    if (initialized_) return;
    
    // 初始化ncurses
    mainWindow_ = initscr();
    if (!mainWindow_) {
        throw std::runtime_error("Failed to initialize ncurses");
    }
    
    // 设置颜色
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_RED, COLOR_BLACK);
        init_pair(5, COLOR_CYAN, COLOR_BLACK);
    }
    
    // 设置输入模式
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // 创建状态窗口
    int height, width;
    getmaxyx(stdscr, height, width);
    statusWindow_ = newwin(Constants::STATUS_WINDOW_HEIGHT, width, height - Constants::STATUS_WINDOW_HEIGHT, 0);
    inputWindow_ = newwin(1, width, height - 1, 0);
    
    // 注册输入处理器
    InputHandler::getInstance().registerHandler(Constants::KeyBindings::QUIT, []() {
        UIManager::getInstance().cleanup();
        exit(0);
    }, "Quit");
    
    InputHandler::getInstance().registerHandler(Constants::KeyBindings::RUN, []() {
        if (LogicCore::getInstance().isAttackRunning()) {
            LogicCore::getInstance().stopAttack();
        } else {
            LogicCore::getInstance().startAttack();
        }
    }, "Run attack");
    
    InputHandler::getInstance().registerHandler(Constants::KeyBindings::ADD_TARGET, []() {
        UIManager::getInstance().showTargetPage();
    }, "Add target");
    
    InputHandler::getInstance().registerHandler(Constants::KeyBindings::NETWORK_CONFIG, []() {
        UIManager::getInstance().showNetworkConfigPage();
    }, "Network config");
    
    InputHandler::getInstance().registerHandler(Constants::KeyBindings::VIEW_STATS, []() {
        UIManager::getInstance().showStatsPage();
    }, "View stats");
    
    InputHandler::getInstance().registerHandler(Constants::KeyBindings::SAVE_CONFIG, []() {
        ConfigManager::getInstance().saveConfig(ConfigManager::getInstance().getConfigPath());
        UIManager::getInstance().showMessage("Configuration saved!");
    }, "Save config");
    
    InputHandler::getInstance().registerHandler(Constants::KeyBindings::LOAD_PRESET, []() {
        UIManager::getInstance().showPresetPage();
    }, "Load preset");
    
    InputHandler::getInstance().registerHandler(Constants::KeyBindings::HELP, []() {
        UIManager::getInstance().showMessage("Press F1-F5 for different functions, Q to quit", 3000);
    }, "Show help");
    
    initialized_ = true;
    Logger::getInstance().log(Log极好的！现在让我继续提供UI模块的剩余代码。

### src/ui/UIManager.cpp（续）
```cpp
    Logger::getInstance().log(LogLevel::INFO, "UI initialized");
}

void UIManager::cleanup() {
    if (!initialized_) return;
    
    InputHandler::getInstance().setActive(false);
    
    if (statusWindow_) {
        delwin(statusWindow_);
        statusWindow_ = nullptr;
    }
    
    if (inputWindow_) {
        delwin(inputWindow_);
        inputWindow_ = nullptr;
    }
    
    if (mainWindow_) {
        endwin();
        mainWindow_ = nullptr;
    }
    
    initialized_ = false;
    Logger::getInstance().log(LogLevel::INFO, "UI cleaned up");
}

void UIManager::run() {
    if (!initialized_) {
        throw std::runtime_error("UI not initialized");
    }
    
    InputHandler::getInstance().setActive(true);
    showMainPage();
    
    while (true) {
        handleInput();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void UIManager::showMainPage() {
    clear();
    drawBorders();
    drawMainMenu();
    refresh();
    currentPage_ = 0;
}

void UIManager::showTargetPage() {
    clear();
    drawBorders();
    mvprintw(2, 2, "Target Configuration");
    mvprintw(4, 2, "Add/Edit targets here");
    refresh();
    currentPage_ = 1;
}

void UIManager::showNetworkConfigPage() {
    clear();
    drawBorders();
    mvprintw(2, 2, "Network Configuration");
    mvprintw(4, 2, "Configure network settings here");
    refresh();
    currentPage_ = 2;
}

void UIManager::showStatsPage() {
    clear();
    drawBorders();
    mvprintw(2, 2, "Statistics");
    mvprintw(4, 2, "View attack statistics here");
    refresh();
    currentPage_ = 3;
}

void UIManager::showPresetPage() {
    clear();
    drawBorders();
    mvprintw(2, 2, "Preset Management");
    mvprintw(4, 2, "Load/Save presets here");
    refresh();
    currentPage_ = 4;
}

void UIManager::updateStats(int packetsSent, int packetsReceived, double attackRate) {
    if (!initialized_ || !statusWindow_) return;
    
    werase(statusWindow_);
    wbkgd(statusWindow_, COLOR_PAIR(1));
    box(statusWindow_, 0, 0);
    
    mvwprintw(statusWindow_, 1, 2, "Sent: %d | Received: %d | Rate: %.2f pps", 
              packetsSent, packetsReceived, attackRate);
    
    wrefresh(statusWindow_);
}

void UIManager::showMessage(const std::string& message, int delayMs) {
    if (!initialized_ || !inputWindow_) return;
    
    werase(inputWindow_);
    wbkgd(inputWindow_, COLOR_PAIR(2));
    mvwprintw(inputWindow_, 0, 2, "%s", message.c_str());
    wrefresh(inputWindow_);
    
    if (delayMs > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        werase(inputWindow_);
        wrefresh(inputWindow_);
    }
}

void UIManager::drawBorders() {
    box(stdscr, 0, 0);
    mvprintw(0, 2, " %s v%s ", Constants::APP_NAME.c_str(), Constants::APP_VERSION.c_str());
}

void UIManager::handleInput() {
    int ch = getch();
    if (ch != ERR) {
        InputHandler::getInstance().processInput(ch);
    }
}

void UIManager::drawMainMenu() {
    mvprintw(2, 2, "Main Menu");
    mvprintw(4, 4, "R - Run Attack");
    mvprintw(5, 4, "F1 - Add/Edit Target");
    mvprintw(6, 4, "F2 - Network Configuration");
    mvprintw(7, 4, "F3 - View Statistics");
    mvprintw(8, 4, "F4 - Save Configuration");
    mvprintw(9, 4, "F5 - Load Preset");
    mvprintw(10, 4, "Q - Quit");
    mvprintw(12, 4, "? - Help");
    
    // 显示当前状态
    if (LogicCore::getInstance().isAttackRunning()) {
        attron(COLOR_PAIR(4));
        mvprintw(14, 4, "Status: ATTACKING");
        attroff(COLOR_PAIR(4));
    } else {
        attron(COLOR_PAIR(2));
        mvprintw(14, 4, "Status: IDLE");
        attroff(COLOR_PAIR(2));
    }
    
    // 显示当前目标
    AttackConfig config = ConfigManager::getInstance().getAttackConfig();
    mvprintw(16, 4, "Target: %s", config.target.c_str());
    mvprintw(17, 4, "Type: %s", config.type.c_str());
    mvprintw(18, 4, "Interface: %s", config.interface.c_str());
}