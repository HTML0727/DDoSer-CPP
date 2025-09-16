#include "ui/ThemeManager.h"
#include "Constants.h"
#include <vector>

ThemeManager& ThemeManager::getInstance() {
    static ThemeManager instance;
    return instance;
}

ThemeManager::ThemeManager() {
    // 初始化主题
    themes_["default"] = {COLOR_WHITE, COLOR_BLACK, COLOR_GREEN, COLOR_YELLOW, COLOR_RED};
    themes_["dark"] = {COLOR_WHITE, COLOR_BLACK, COLOR_CYAN, COLOR_YELLOW, COLOR_RED};
    themes_["light"] = {COLOR_BLACK, COLOR_WHITE, COLOR_BLUE, COLOR_MAGENTA, COLOR_RED};
    
    currentTheme_ = "default";
}

void ThemeManager::applyTheme(const std::string& themeName) {
    auto it = themes_.find(themeName);
    if (it == themes_.end()) {
        return; // 主题不存在
    }
    
    currentTheme_ = themeName;
    const ColorScheme& scheme = it->second;
    
    // 设置颜色对
    init_pair(Constants::ColorPairs::DEFAULT, scheme.textColor, scheme.backgroundColor);
    init_pair(Constants::ColorPairs::HIGHLIGHT, scheme.highlightColor, scheme.backgroundColor);
    init_pair(Constants::ColorPairs::WARNING, scheme.warningColor, scheme.backgroundColor);
    init_pair(Constants::ColorPairs::ERROR, scheme.errorColor, scheme.backgroundColor);
    init_pair(Constants::ColorPairs::SUCCESS, scheme.highlightColor, scheme.backgroundColor);
    
    // 应用默认颜色
    bkgd(COLOR_PAIR(Constants::ColorPairs::DEFAULT));
}

ColorScheme ThemeManager::getCurrentTheme() const {
    auto it = themes_.find(currentTheme_);
    if (it != themes_.end()) {
        return it->second;
    }
    return themes_.begin()->second;
}

std::vector<std::string> ThemeManager::getAvailableThemes() const {
    std::vector<std::string> themeNames;
    for (const auto& pair : themes_) {
        themeNames.push_back(pair.first);
    }
    return themeNames;
}