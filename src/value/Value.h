#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>

#include "ast/statement/Statement.h"

namespace dont_use_script::executor {
class Context;
}

namespace dont_use_script::value {

enum class ValueType {
    kArrayValue,
    kBoolValue,
    kDoubleValue,
    kFunctionValue,
    kStringValue,
    kNullValue,
};

inline std::string type_to_string(ValueType type) {
    switch (type) {
        case ValueType::kArrayValue: return "Array";
        case ValueType::kBoolValue: return "Bool";
        case ValueType::kDoubleValue: return "Double";
        case ValueType::kFunctionValue: return "Function";
        case ValueType::kStringValue: return "String";
        case ValueType::kNullValue: return "Null";
    }
    return "";
}

class Value;

using ValuePtr = std::shared_ptr<Value>;

class Value {
public:
    [[nodiscard]] virtual ValueType GetValueType() = 0;
    [[nodiscard]] virtual bool AsBool() = 0;
    [[nodiscard]] virtual double AsDouble() = 0;
    [[nodiscard]] virtual std::string& AsString() = 0;
    [[nodiscard]] virtual std::vector<ValuePtr>& AsArray() = 0;
    [[nodiscard]] virtual ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) = 0;

    virtual ~Value() = default;
};


class Bool final : public Value {
public:
    explicit Bool(bool value);

    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) override;

private:
    bool value_;
    std::string str_value_;
};


class Double final : public Value {
public:
    explicit Double(double num);

    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) override;

private:
    double num_;
    std::string str_value_;
};


class String final: public Value {
public:
    explicit String(const std::string& value);

    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) override;
private:
    std::string value_;
};


class Array final: public Value {
public:
    explicit Array();
    explicit Array(std::vector<ValuePtr>);

    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) override;

private:
    std::vector<ValuePtr> array_;
    std::string str_;
};


class Null: public Value {
public:
    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) override;
private:
    std::string str_;
    std::vector<ValuePtr> array_;
};


class Function: public Value {
public:
    Function(std::unique_ptr<ast::Statement> function_body,
                  std::vector<std::string> arguments);

    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) override;

private:
    std::unique_ptr<ast::Statement> scope_;
    std::vector<std::string> arguments_;
};


class SystemFunction final : public Value {
public:
    template<class Function>
    SystemFunction(Function function)
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

    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) override {
        return function_(args);
    }

private:
    std::function<ValuePtr(const std::vector<ValuePtr>&)> function_;
};

} // namespace dont_use_script::value