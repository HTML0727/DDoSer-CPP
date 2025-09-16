#pragma once
#include "SocketWrapper.h"
#include <vector>
#include <cstdint>

class RawSocket : public SocketWrapper {
public:
    RawSocket(int protocol);
    ~RawSocket() = default;
    
    bool sendPacket(const std::vector<uint8_t>& packet);
    bool sendPacket(const void* data, size_t length);
    
    // 设置IP头部包含选项
    void setIPHeaderIncluded(bool include) { ipHeaderIncluded_ = include; }
    
private:
    bool ipHeaderIncluded_;
};