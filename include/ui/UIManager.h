#pragma once
#include <ncurses.h>
#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <thread>
#include <atomic>

class UIManager {
public:
    static UIManager& getInstance();
    
    void initialize();
    void cleanup();
    void run();
    
    void showMainPage();
    void showTargetPage();
    void showNetworkConfigPage();
    void showStatsPage();
    void showPresetPage();
    
    void updateStats(int packetsSent, int packetsReceived, double attackRate);
    void showMessage(const std::string& message, int delayMs = 2000);
    
    // 禁止拷贝和移动
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(UIManager&&) = delete;
    
private:
    UIManager();
    ~UIManager();
    
    WINDOW* mainWindow_;
    WINDOW* statusWindow_;
    WINDOW* inputWindow_;
    
    bool initialized_;
    int currentPage_;
    
    void drawBorders();
    void handleInput();
    void drawMainMenu();
};