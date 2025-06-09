#include "../include/DoubleValue.h"

#include <stdexcept>
#include <string>


DoubleValue::DoubleValue(double num)
    : num_(num) {}

ValueType DoubleValue::GetValueType() const {
    return ValueType::kDoubleValue;
}

double DoubleValue::AsDouble() const {
    return num_;
}

bool DoubleValue::AsBool() const {
    return num_ != 0;
}

std::string DoubleValue::AsString() const {
    return std::to_string(num_);
}

std::vector<std::unique_ptr<Value>> DoubleValue::AsArray() const {
    throw std::runtime_error("Number cannot be used as array");
}
