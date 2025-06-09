#include "../include/StringValue.h"


#include <stdexcept>


StringValue::StringValue(const std::string& value)
    : value_(value) {}

ValueType StringValue::GetValueType() const {
    return ValueType::kStringValue;
}

bool StringValue::AsBool() const {
    return !value_.empty();
}

std::string StringValue::AsString() const {
    return value_;
}

std::vector<std::unique_ptr<Value>> StringValue::AsArray() const {
    throw std::runtime_error("String cannot be used as array");
}

double StringValue::AsDouble() const {
    throw std::runtime_error("String cannot be used as double");
}
