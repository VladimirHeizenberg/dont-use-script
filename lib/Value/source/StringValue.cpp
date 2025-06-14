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

const std::string& StringValue::AsString() const {
    return value_;
}

const std::vector<std::unique_ptr<Value>>& StringValue::AsArray() const {
    throw std::runtime_error("String cannot be used as array");
}

double StringValue::AsDouble() const {
    throw std::runtime_error("String cannot be used as double");
}

ValuePtr StringValue::AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) const {
    throw std::runtime_error("String cannot be used as function call");
}