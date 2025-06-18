#pragma once

#include <functional>
#include <stdexcept>

#include "src/value/headers/ValueInterface.h"

class Context; // declaration to avoid cyclic includes in Executor/Context

class SystemFunctionValue final : public Value {
public:
    template<class Function>
    SystemFunctionValue(Function function)
        : function_(std::move(function)) {}

    [[nodiscard]] ValueType GetValueType() override {
        return ValueType::kFunctionValue;
    }

    [[nodiscard]] bool AsBool() override {
        throw std::runtime_error("Function cannot be used as bool");
    }

    [[nodiscard]] double AsDouble() override {
        throw std::runtime_error("Function cannot be used as double");
    }

    [[nodiscard]] std::string& AsString() override {
        throw std::runtime_error("Function cannot be used as string");
    }

    [[nodiscard]] std::vector<ValuePtr>& AsArray() override {
        throw std::runtime_error("Function cannot be used as array");
    }

    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) override {
        return function_(args);
    }

private:
    std::function<ValuePtr(const std::vector<ValuePtr>&)> function_;
};
