#include "../include/BoolValue.h"


#include <stdexcept>


BoolValue::BoolValue(bool value)
    : value_(value) {}

ValueType BoolValue::GetValueType() const {
    return ValueType::kBoolValue;
}

bool BoolValue::AsBool() const {
    return value_;
}

double BoolValue::AsDouble() const {
    return value_ ? 1 : 0;
}

std::string BoolValue::AsString() const {
    return value_ ? "true" : "false";
}

std::vector<std::unique_ptr<Value>> BoolValue::AsArray() const {
    throw std::runtime_error("Bool cannot be used as array");
}