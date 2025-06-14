#include "../include/BoolValue.h"


#include <stdexcept>


BoolValue::BoolValue(bool value)
    : value_(value)
    , str_value_(value ? "true" : "false") {}

ValueType BoolValue::GetValueType() const {
    return ValueType::kBoolValue;
}

bool BoolValue::AsBool() const {
    return value_;
}

double BoolValue::AsDouble() const {
    return value_ ? 1 : 0;
}

const std::string& BoolValue::AsString() const {
    return str_value_;
}

const std::vector<std::unique_ptr<Value>>& BoolValue::AsArray() const {
    throw std::runtime_error("Bool cannot be used as array");
}

ValuePtr BoolValue::AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) const {
    throw std::runtime_error("Bool cannot be used as function call");
}