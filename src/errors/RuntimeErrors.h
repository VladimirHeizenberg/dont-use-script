#pragma once

#include "InterpreterError.h"


namespace dont_use_script::errors {

class RuntimeError: public InterpreterError {
public:
    explicit RuntimeError(const std::string& message)
    : InterpreterError(message) {}
};


class TypeError: public RuntimeError {
public:
    explicit TypeError(const std::string& message)
    : RuntimeError("TypeError: " + message) {}
};


class IndexError: public RuntimeError {
public:
    explicit IndexError(const std::string& message)
    : RuntimeError("IndexError: " + message) {}
};


class NameError: public RuntimeError {
public:
    explicit NameError(const std::string& message)
    : RuntimeError("NameError: " + message) {}
};


} // namespace dont_use_script::errors