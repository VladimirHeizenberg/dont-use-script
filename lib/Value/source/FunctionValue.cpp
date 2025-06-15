#include "../include/FunctionValue.h"

#include <complex>

FunctionValue::FunctionValue(std::unique_ptr<StatementAST> function_body,
                             std::vector<std::string> arguments)
    : scope_(std::move(function_body))
    , arguments_(std::move(arguments)) {}


ValueType FunctionValue::GetValueType() const {
    return ValueType::kFunctionValue;
}


double FunctionValue::AsDouble() const {
    throw std::runtime_error("Function cannot be used as double");
}

bool FunctionValue::AsBool() const {
    throw std::runtime_error("Function cannot be used as bool");
}

const std::string& FunctionValue::AsString() const {
    throw std::runtime_error("Function cannot be used as string");
}

const std::vector<std::unique_ptr<Value>>& FunctionValue::AsArray() const {
    throw std::runtime_error("Function cannot be used as array");
}

ValuePtr FunctionValue::AsFunctionCall(const std::vector<ValuePtr> &args_values, Context& context) const {
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
    return nullptr;
}


