#pragma once
#include <exception>
#include <string>

class DDoSerException : public std::exception {
public:
    explicit DDoSerException(const std::string& message) : message_(message) {}
    virtual ~DDoSerException() = default;
    
    const char* what() const noexcept override { return message_.c_str(); }
    
private:
    std::string message_;
};

class NetworkException : public DDoSerException {
public:
    explicit NetworkException(const std::string& message) : DDoSerException(message) {}
};

class ConfigException : public DDoSerException {
public:
    explicit ConfigException(const std::string& message) : DDoSerException(message) {}
};

class UIException : public DDoSerException {
public:
    explicit UIException(const std::string& message) : DDoSerException(message) {}
};

class AttackException : public DDoSerException {
public:
    explicit AttackException(const std::string& message) : DDoSerException(message) {}
};