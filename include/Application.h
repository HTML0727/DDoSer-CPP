#pragma once
#include <string>
#include <memory>
#include <map>

class Application {
public:
    Application(int argc, char* argv[]);
    ~Application();
    
    int run();
    void stop();
    
    static Application& getInstance();
    
    // 命令行参数
    std::string getArgument(const std::string& name, const std::string& defaultValue = "") const;
    bool hasArgument(const std::string& name) const;
    bool isDaemon() const { return daemonMode_; }
    
    // 禁止拷贝和移动
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;
    
private:
    bool initialize();
    void cleanup();
    void parseArguments(int argc, char* argv[]);
    void showHelp() const;
    
    std::map<std::string, std::string> arguments_;
    bool daemonMode_;
    bool running_;
    
    static Application* instance_;
};