#include "ui/InputHandler.h"
#include "util/Logger.h"

InputHandler& InputHandler::getInstance() {
    static InputHandler instance;
    return instance;
}

InputHandler::InputHandler() : active_(false) {}

void InputHandler::registerHandler(int key, HandlerFunc handler, const std::string& description) {
    handlers_[key] = std::make_pair(handler, description);
}

void InputHandler::processInput(int key) {
    if (!active_) return;
    
    auto it = handlers_.find(key);
    if (it != handlers_.end()) {
        try {
            it->second.first();
        } catch (const std::exception& e) {
            Logger::getInstance().log(LogLevel::ERROR, "Input handler error: " + std::string(e.what()));
        }
    } else {
        Logger::getInstance().log(LogLevel::DEBUG, "Unhandled key: " + std::to_string(key));
    }
}

std::vector<std::pair<int, std::string>> InputHandler::getRegisteredKeys() const {
    std::vector<std::pair<int, std::string>> result;
    for (const auto& pair : handlers_) {
        result.emplace_back(pair.first, pair.second.second);
    }
    return result;
}