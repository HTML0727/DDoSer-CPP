#include "util/Helpers.h"
#include <algorithm>
#include <cctype>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>

namespace helpers {
    std::vector<std::string> splitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        if (start == std::string::npos) return "";
        
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(start, end - start + 1);
    }
    
    bool startsWith(const std::string& str, const std::string& prefix) {
        if (prefix.size() > str.size()) return false;
        return std::equal(prefix.begin(), prefix.end(), str.begin());
    }
    
    bool endsWith(const std::string& str, const std::string& suffix) {
        if (suffix.size() > str.size()) return false;
        return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
    }
    
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }
    
    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        return result;
    }
    
    bool isRoot() {
        return geteuid() == 0;
    }
    
    void daemonize() {
        pid_t pid = fork();
        
        if (pid < 0) {
            exit(EXIT_FAILURE);
        }
        
        if (pid > 0) {
            exit(EXIT_SUCCESS);
        }
        
        if (setsid() < 0) {
            exit(EXIT_FAILURE);
        }
        
        signal(SIGCHLD, SIG_IGN);
        signal(SIGHUP, SIG_IGN);
        
        pid = fork();
        
        if (pid < 0) {
            exit(EXIT_FAILURE);
        }
        
        if (pid > 0) {
            exit(EXIT_SUCCESS);
        }
        
        umask(0);
        
        for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
            close(x);
        }
    }
}