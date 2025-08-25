#pragma once

#include <stdexcept>

namespace itmo_script::errors {

class InterpreterError: public std::runtime_error {
public:
    explicit InterpreterError(const std::string& message)
    : std::runtime_error(message) {}
};

}