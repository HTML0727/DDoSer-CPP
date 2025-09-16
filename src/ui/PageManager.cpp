#include "ui/PageManager.h"

PageManager& PageManager::getInstance() {
    static PageManager instance;
    return instance;
}

PageManager::PageManager() : currentPage_(-1) {}

void PageManager::registerPage(int id, PageRenderer renderer, const std::string& name) {
    pages_[id] = std::make_pair(renderer, name);
}

void PageManager::showPage(int id) {
    auto it = pages_.find(id);
    if (it == pages_.end()) {
        return; // 页面不存在
    }
    
    pageHistory_.push_back(currentPage_);
    currentPage_ = id;
    it->second.first();
}

void PageManager::showPreviousPage() {
    if (pageHistory_.empty()) {
        return;
    }
    
    int previousPage = pageHistory_.back();
    pageHistory_.pop_back();
    showPage(previousPage);
}

std::string PageManager::getCurrentPageName() const {
    auto it = pages_.find(currentPage_);
    if (it != pages_.end()) {
        return it->second.second;
    }
    return "Unknown";
}

void PageManager::navigateTo(int pageId) {
    showPage(pageId);
}

void PageManager::navigateBack() {
    showPreviousPage();
}