#pragma once
#include <functional>
#include <string>
#include <vector>
#include <map>

class PageManager {
public:
    using PageRenderer = std::function<void()>;
    
    static PageManager& getInstance();
    
    void registerPage(int id, PageRenderer renderer, const std::string& name);
    void showPage(int id);
    void showPreviousPage();
    int getCurrentPage() const { return currentPage_; }
    std::string getCurrentPageName() const;
    
    // 导航
    void navigateTo(int pageId);
    void navigateBack();
    
    // 禁止拷贝和移动
    PageManager(const PageManager&) = delete;
    PageManager& operator=(const PageManager&) = delete;
    PageManager(PageManager&&) = delete;
    PageManager& operator=(PageManager&&) = delete;
    
private:
    PageManager();
    
    std::map<int, std::pair<PageRenderer, std::string>> pages_;
    int currentPage_;
    std::vector<int> pageHistory_;
};