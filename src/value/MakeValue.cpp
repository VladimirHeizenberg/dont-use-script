#include "src/value/headers/MakeValue.h"

#include "headers/SystemFunction.h"
#include "src/value/headers/StringValue.h"
#include "src/value/headers/DoubleValue.h"
#include "src/value/headers/BoolValue.h"
#include "src/value/headers/FunctionValue.h"
#include "src/value/headers/NullValue.h"
#include "src/value/headers/ArrayValue.h"


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

ValuePtr MakeSystemFunctionValue(std::function<ValuePtr(const std::vector<ValuePtr>&)> function) {
    return std::make_shared<SystemFunctionValue>(std::move(function));
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