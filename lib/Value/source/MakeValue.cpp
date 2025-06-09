#include "../include/StringValue.h"
#include "../include/DoubleValue.h"
#include "../include/BoolValue.h"

ValuePtr MakeStringValue(const std::string& value) {
    return std::make_shared<StringValue>(value);
}

ValuePtr MakeDoubleValue(double num) {
    return std::make_shared<DoubleValue>(num);
}

ValuePtr MakeBoolValue(bool value) {
    return std::make_shared<BoolValue>(value);
}