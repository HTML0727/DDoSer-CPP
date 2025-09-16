#include "Application.h"
#include "util/Logger.h"

int main(int argc, char* argv[]) {
    try {
        Application app(argc, argv);
        return app.run();
    } catch (const std::exception& e) {
        Logger::getInstance().log(LogLevel::ERROR, "Fatal error: " + std::string(e.what()));
        return 1;
    }
}