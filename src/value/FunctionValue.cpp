#include "src/value/headers/FunctionValue.h"

#include <stdexcept>

#include "src/value/headers/MakeValue.h"

FunctionValue::FunctionValue(std::unique_ptr<StatementAST> function_body,
                             std::vector<std::string> arguments)
    : scope_(std::move(function_body))
    , arguments_(std::move(arguments)) {}


ValueType FunctionValue::GetValueType() {
    return ValueType::kFunctionValue;
}


double FunctionValue::AsDouble() {
    throw std::runtime_error("Function cannot be used as double");
}

bool FunctionValue::AsBool() {
    throw std::runtime_error("Function cannot be used as bool");
}

std::string& FunctionValue::AsString() {
    throw std::runtime_error("Function cannot be used as string");
}

std::vector<ValuePtr>& FunctionValue::AsArray() {
    throw std::runtime_error("Function cannot be used as array");
}

ValuePtr FunctionValue::AsFunctionCall(const std::vector<ValuePtr> &args_values, Context& context) {
    if (args_values.size() != arguments_.size()) {
        throw std::runtime_error("Function called with incorrect number of arguments");
    }
    VariablesTable table(context.table());
    for (int i = 0; i < args_values.size(); ++i) {
        table.Set(arguments_[i], args_values[i]);
    }

    Context scope_context(context.input(), context.output(), table);

    auto result = scope_->execute(scope_context);

    if (result.result == StatementResult::kReturn) {
        return result.value;
    }
    return MakeNullValue();
}


