#include "../include/MakeValue.h"


#include "../include/StringValue.h"
#include "../include/DoubleValue.h"
#include "../include/BoolValue.h"
#include "../include/FunctionValue.h"
#include "../include/NullValue.h"
#include "Value/include/ArrayValue.h"

ValuePtr MakeStringValue(const std::string& value) {
    return std::make_shared<StringValue>(value);
}

ValuePtr MakeDoubleValue(double num) {
    return std::make_shared<DoubleValue>(num);
}

ValuePtr MakeBoolValue(bool value) {
    return std::make_shared<BoolValue>(value);
}

ValuePtr MakeFunctionValue(std::unique_ptr<StatementAST> function_body,
                           std::vector<std::string> arguments) {
    return std::make_shared<FunctionValue>(std::move(function_body), std::move(arguments));
}

ValuePtr MakeNullValue() {
    return std::make_shared<NullValue>();
}

ValuePtr MakeArrayValue() {
    return std::make_shared<ArrayValue>();
}

ValuePtr MakeArrayValue(std::vector<ValuePtr> values) {
    return std::make_shared<ArrayValue>(std::move(values));
}