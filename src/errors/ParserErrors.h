#pragma once

#include "InterpreterError.h"
#include "parser/Location.h"

namespace dont_use_script::errors {

class SyntaxError: public InterpreterError {
public:
    explicit SyntaxError(const std::string& message, parser::Location location)
    : InterpreterError("SyntaxError: " + message) {}
};


class UnexpectedTokenError : public InterpreterError {
public:
    explicit UnexpectedTokenError(const std::string& msg)
    : InterpreterError("UnexpectedToken: " + msg) {}
};

} // namespace dont_use_script::errors