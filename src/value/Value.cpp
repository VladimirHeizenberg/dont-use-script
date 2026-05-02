#include "Value.h"


#include <stdexcept>
#include <string>
#include <sstream>

#include "MakeValue.h"

#include "executor/VariablesTable.h"
#include "ast/statement/Statement.h"

namespace dont_use_script::value {

/* bool value */

Bool::Bool(bool value)
    : value_(value)
    , str_value_(value ? "true" : "false") {}

ValueType Bool::GetValueType() {
    return ValueType::kBoolValue;
}

bool Bool::AsBool() {
    return value_;
}

double Bool::AsDouble() {
    return value_ ? 1 : 0;
}

std::string& Bool::AsString() {
    return str_value_;
}

std::vector<ValuePtr>& Bool::AsArray() {
    throw std::runtime_error("Bool cannot be used as array");
}

ValuePtr Bool::AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) {
    throw std::runtime_error("Bool cannot be used as function call");
}

/* double value */

std::string to_string_trimmed(double value) {
    std::ostringstream oss;
    oss << value;
    std::string result = oss.str();

    if (result.find('.') != std::string::npos) {
        result.erase(result.find_last_not_of('0') + 1);
        if (result.back() == '.')
            result.pop_back();
    }

    return result;
}

Double::Double(double num)
    : num_(num)
    , str_value_(to_string_trimmed(num_)) {}

ValueType Double::GetValueType() {
    return ValueType::kDoubleValue;
}

double Double::AsDouble() {
    return num_;
}

bool Double::AsBool() {
    return num_ != 0;
}

std::string& Double::AsString() {
    return str_value_;
}

std::vector<ValuePtr>& Double::AsArray() {
    throw std::runtime_error("Number cannot be used as array");
}

ValuePtr Double::AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) {
    throw std::runtime_error("Number cannot be used as function call");
}

/* string value */

String::String(const std::string& value)
    : value_(value) {}

ValueType String::GetValueType() {
    return ValueType::kStringValue;
}

bool String::AsBool() {
    return !value_.empty();
}

std::string& String::AsString() {
    return value_;
}

std::vector<ValuePtr>& String::AsArray() {
    throw std::runtime_error("String cannot be used as array");
}

double String::AsDouble() {
    throw std::runtime_error("String cannot be used as double");
}

ValuePtr String::AsFunctionCall(const std::vector<ValuePtr>& args, executor::Context& context) {
    throw std::runtime_error("String cannot be used as function call");
}

/* array */

Array::Array() = default;

Array::Array(std::vector<ValuePtr> values)
: array_(std::move(values)) {}


ValueType Array::GetValueType() {
    return ValueType::kArrayValue;
}

bool Array::AsBool() {
    return !array_.empty();
}

double Array::AsDouble() {
    throw std::runtime_error("Array cannot be used as number");
}

std::string& Array::AsString() {
    str_.clear();
    str_ += '[';
    for (auto& element : array_) {
        if (element->GetValueType() == ValueType::kStringValue) {
            str_ += '\"';
            str_ += element->AsString();
            str_ += "\", ";
            continue;
        }
        str_ += element->AsString();
        str_ += ", ";
    }
    str_.pop_back();
    str_.pop_back();
    str_ += "]";
    return str_;
}

std::vector<ValuePtr>& Array::AsArray() {
    return array_;
}

ValuePtr Array::AsFunctionCall(const std::vector<ValuePtr> &args, executor::Context& context) {
    throw std::runtime_error("Array cannot be used as function");
}


/* Null */

ValueType Null::GetValueType() {
    return ValueType::kNullValue;
}

bool Null::AsBool() {
    return false;
}

double Null::AsDouble() {
    return 0;
}

std::string& Null::AsString() {
    return str_;
}

std::vector<ValuePtr>& Null::AsArray() {
    return array_;
}

ValuePtr Null::AsFunctionCall(const std::vector<ValuePtr> &args, executor::Context& context) {
    throw std::runtime_error("NullType cannot be used as function");
}

/* Function */

Function::Function(std::unique_ptr<ast::Statement> function_body,
                             std::vector<std::string> arguments)
    : scope_(std::move(function_body))
    , arguments_(std::move(arguments)) {}


ValueType Function::GetValueType() {
    return ValueType::kFunctionValue;
}


double Function::AsDouble() {
    throw std::runtime_error("Function cannot be used as double");
}

bool Function::AsBool() {
    throw std::runtime_error("Function cannot be used as bool");
}

std::string& Function::AsString() {
    throw std::runtime_error("Function cannot be used as string");
}

std::vector<ValuePtr>& Function::AsArray() {
    throw std::runtime_error("Function cannot be used as array");
}

ValuePtr Function::AsFunctionCall(const std::vector<ValuePtr> &args_values, executor::Context& context) {
    if (args_values.size() != arguments_.size()) {
        throw std::runtime_error("Function called with incorrect number of arguments");
    }
    executor::VariablesTable table(context.table());
    for (int i = 0; i < args_values.size(); ++i) {
        table.Set(arguments_[i], args_values[i]);
    }

    executor::Context scope_context(context.input(), context.output(), table);

    auto result = scope_->Execute(scope_context);

    if (result.result == ast::StatementResult::kReturn) {
        return result.value;
    }
    return MakeNull();
}

} // namespace dont_use_script::value