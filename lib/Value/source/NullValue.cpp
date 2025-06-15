#include "../include/NullValue.h"

ValueType NullValue::GetValueType() const {
    return ValueType::kNullValue;
}

bool NullValue::AsBool() const {
    return false;
}

double NullValue::AsDouble() const {
    return 0;
}

const std::string& NullValue::AsString() const {
    return str_;
}

const std::vector<std::unique_ptr<Value>>& NullValue::AsArray() const {
    return array_;
}

ValuePtr NullValue::AsFunctionCall(const std::vector<ValuePtr> &args, Context &context) const {
    throw std::runtime_error("NullType cannot be used as function");
}




