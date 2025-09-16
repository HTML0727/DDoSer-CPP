#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <memory>

class SocketWrapper {
public:
    SocketWrapper(int domain, int type, int protocol);
    ~SocketWrapper();
    
    bool bind(const std::string& ip, uint16_t port);
    bool connect(const std::string& ip, uint16_t port);
    ssize_t send(const void* buffer, size_t length, int flags = 0);
    ssize_t receive(void* buffer, size_t length, int flags = 0);
    bool setOption(int level, int option, const void* value, socklen_t len);
    bool setNonBlocking(bool nonBlocking);
    
    int getDescriptor() const { return sockfd_; }
    bool isValid() const { return sockfd_ >= 0; }
    
    // 禁止拷贝
    SocketWrapper(const SocketWrapper&) = delete;
    SocketWrapper& operator=(const SocketWrapper&) = delete;
    
    // 允许移动
    SocketWrapper(SocketWrapper&& other) noexcept;
    SocketWrapper& operator=(SocketWrapper&& other) noexcept;
    
private:
    int sockfd_;
};