#include "../include/ArrayValue.h"


#include <iostream>
#include <stdexcept>
#include <string>

ArrayValue::ArrayValue() = default;

ArrayValue::ArrayValue(std::vector<ValuePtr> values)
: array_(std::move(values)) {}


ValueType ArrayValue::GetValueType() {
    return ValueType::kArrayValue;
}

bool ArrayValue::AsBool() {
    return !array_.empty();
}

double ArrayValue::AsDouble() {
    throw std::runtime_error("Array cannot be used as number");
}

std::string& ArrayValue::AsString() {
    str_.clear();
    str_ += '[';
    for (auto& element : array_) {
        // std::cerr << element->AsString() << std::endl;
        // TODO:
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

std::vector<ValuePtr>& ArrayValue::AsArray() {
    return array_;
}

ValuePtr ArrayValue::AsFunctionCall(const std::vector<ValuePtr> &args, Context &context) {
    throw std::runtime_error("Array cannot be used as function");
}



