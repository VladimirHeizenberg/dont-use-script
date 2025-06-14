#pragma once

#include "Expression.h"

class FunctionCallExpression: public ExpressionAST {
public:
    FunctionCallExpression(std::string name, std::vector<std::unique_ptr<ExpressionAST>> arguments)
    : name_(std::move(name))
    , arguments_(std::move(arguments)) {}

    ValuePtr evaluate(Context &context) override {
        if (!context.table().Contains(name_)) throw std::runtime_error("Function '" + name_ + "' is not defined\n");
        std::vector<ValuePtr> result_arguments;
        for (auto& argument : arguments_) {
            result_arguments.push_back(argument->evaluate(context));
        }
        return context.table().Get(name_)->AsFunctionCall(result_arguments, context);
    }

private:
    std::string name_;
    std::vector<std::unique_ptr<ExpressionAST>> arguments_;
};