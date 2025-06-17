#include "../include/DoubleValue.h"

#include <stdexcept>
#include <string>
#include <sstream>

std::string to_string_trimmed(double value) {
    std::ostringstream oss;
    oss << value;
    std::string result = oss.str();

    if (result.find('.') != std::string::npos) {
        result.erase(result.find_last_not_of('0') + 1);
        if (result.back() == '.')
            result.pop_back();
    }

    return result;
}

DoubleValue::DoubleValue(double num)
    : num_(num)
    , str_value_(to_string_trimmed(num_)) {}

ValueType DoubleValue::GetValueType() {
    return ValueType::kDoubleValue;
}

double DoubleValue::AsDouble() {
    return num_;
}

bool DoubleValue::AsBool() {
    return num_ != 0;
}

std::string& DoubleValue::AsString() {
    return str_value_;
}

std::vector<ValuePtr>& DoubleValue::AsArray() {
    throw std::runtime_error("Number cannot be used as array");
}

ValuePtr DoubleValue::AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) {
    throw std::runtime_error("Number cannot be used as function call");
}