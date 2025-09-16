#include "network/SocketWrapper.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <cstring>
#include "util/Logger.h"

SocketWrapper::SocketWrapper(int domain, int type, int protocol) {
    sockfd_ = socket(domain, type, protocol);
    if (sockfd_ < 0) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to create socket");
        throw std::runtime_error("Failed to create socket");
    }
}

SocketWrapper::~SocketWrapper() {
    if (sockfd_ >= 0) {
        close(sockfd_);
    }
}

bool SocketWrapper::bind(const std::string& ip, uint16_t port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    
    if (::bind(sockfd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to bind socket");
        return false;
    }
    
    return true;
}

bool SocketWrapper::connect(const std::string& ip, uint16_t port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    
    if (::connect(sockfd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to connect socket");
        return false;
    }
    
    return true;
}

ssize_t SocketWrapper::send(const void* buffer, size_t length, int flags) {
    ssize_t result = ::send(sockfd_, buffer, length, flags);
    if (result < 0) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to send data");
    }
    return result;
}

ssize_t SocketWrapper::receive(void* buffer, size_t length, int flags) {
    ssize_t result = ::recv(sockfd_, buffer, length, flags);
    if (result < 0) {
        Logger::getInstance().log(LogLevel::ERROR, "Failed to receive data");
    }
    return result;
}

bool SocketWrapper::setOption(int level, int option, const void* value, socklen_t len) {
    return setsockopt(sockfd_, level, option, value, len) == 0;
}

bool SocketWrapper::setNonBlocking(bool nonBlocking) {
    int flags = fcntl(sockfd_, F_GETFL, 0);
    if (flags < 0) return false;
    
    if (nonBlocking) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    
    return fcntl(sockfd_, F_SETFL, flags) == 0;
}

SocketWrapper::SocketWrapper(SocketWrapper&& other) noexcept : sockfd_(other.sockfd_) {
    other.sockfd_ = -1;
}

SocketWrapper& SocketWrapper::operator=(SocketWrapper&& other) noexcept {
    if (this != &other) {
        if (sockfd_ >= 0) {
            close(sockfd_);
        }
        sockfd_ = other.sockfd_;
        other.sockfd_ = -1;
    }
    return *this;
}