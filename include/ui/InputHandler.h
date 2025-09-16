#pragma once
#include <functional>
#include <string>
#include <vector>
#include <map>

class InputHandler {
public:
    using HandlerFunc = std::function<void()>;
    
    static InputHandler& getInstance();
    
    void registerHandler(int key, HandlerFunc handler, const std::string& description = "");
    void processInput(int key);
    void setActive(bool active) { active_ = active; }
    bool isActive() const { return active_; }
    
    std::vector<std::pair<int, std::string>> getRegisteredKeys() const;
    
    // 禁止拷贝和移动
    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;
    InputHandler(InputHandler&&) = delete;
    InputHandler& operator=(InputHandler&&) = delete;
    
private:
    InputHandler();
    
    std::map<int, std::pair<HandlerFunc, std::string>> handlers_;
    bool active_;
};