#include "network/RawSocket.h"
#include <stdexcept>
#include "util/Logger.h"

RawSocket::RawSocket(int protocol) 
    : SocketWrapper(AF_INET, SOCK_RAW, protocol), ipHeaderIncluded_(true) {}

bool RawSocket::sendPacket(const std::vector<uint8_t>& packet) {
    return sendPacket(packet.data(), packet.size());
}

bool RawSocket::sendPacket(const void* data, size_t length) {
    ssize_t result = send(data, length, 0);
    return result == static_cast<ssize_t>(length);
}