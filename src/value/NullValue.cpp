#include "src/value/headers/NullValue.h"

#include <stdexcept>


ValueType NullValue::GetValueType() {
    return ValueType::kNullValue;
}

bool NullValue::AsBool() {
    return false;
}

double NullValue::AsDouble() {
    return 0;
}

std::string& NullValue::AsString() {
    return str_;
}

std::vector<ValuePtr>& NullValue::AsArray() {
    return array_;
}

ValuePtr NullValue::AsFunctionCall(const std::vector<ValuePtr> &args, Context &context) {
    throw std::runtime_error("NullType cannot be used as function");
}




