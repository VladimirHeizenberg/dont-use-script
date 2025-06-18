#include "src/value/headers/StringValue.h"


#include <stdexcept>


StringValue::StringValue(const std::string& value)
    : value_(value) {}

ValueType StringValue::GetValueType() {
    return ValueType::kStringValue;
}

bool StringValue::AsBool() {
    return !value_.empty();
}

std::string& StringValue::AsString() {
    return value_;
}

std::vector<ValuePtr>& StringValue::AsArray() {
    throw std::runtime_error("String cannot be used as array");
}

double StringValue::AsDouble() {
    throw std::runtime_error("String cannot be used as double");
}

ValuePtr StringValue::AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) {
    throw std::runtime_error("String cannot be used as function call");
}