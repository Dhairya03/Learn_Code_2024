#ifndef ATMEXCEPTION_H
#define ATMEXCEPTION_H

#include <stdexcept>
#include <string>

class ATMException : public std::exception {
    std::string message;
public:
    explicit ATMException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif 