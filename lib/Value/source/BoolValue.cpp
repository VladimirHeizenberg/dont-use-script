#include "../include/BoolValue.h"


#include <stdexcept>


BoolValue::BoolValue(bool value)
    : value_(value)
    , str_value_(value ? "true" : "false") {}

ValueType BoolValue::GetValueType() {
    return ValueType::kBoolValue;
}

bool BoolValue::AsBool() {
    return value_;
}

double BoolValue::AsDouble() {
    return value_ ? 1 : 0;
}

std::string& BoolValue::AsString() {
    return str_value_;
}

std::vector<ValuePtr>& BoolValue::AsArray() {
    throw std::runtime_error("Bool cannot be used as array");
}

ValuePtr BoolValue::AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) {
    throw std::runtime_error("Bool cannot be used as function call");
}