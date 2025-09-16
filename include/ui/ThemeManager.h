#pragma once
#include <ncurses.h>
#include <map>
#include <string>

struct ColorScheme {
    short textColor;
    short backgroundColor;
    short highlightColor;
    short warningColor;
    short errorColor;
};

class ThemeManager {
public:
    static ThemeManager& getInstance();
    
    void applyTheme(const std::string& themeName);
    ColorScheme getCurrentTheme() const;
    std::vector<std::string> getAvailableThemes() const;
    
    // 禁止拷贝和移动
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
    ThemeManager(ThemeManager&&) = delete;
    ThemeManager& operator=(ThemeManager&&) = delete;
    
private:
    ThemeManager();
    std::map<std::string, ColorScheme> themes_;
    std::string currentTheme_;
};