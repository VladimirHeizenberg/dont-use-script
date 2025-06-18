#pragma once

#include "src/AST/Expression.h"

class FunctionCallExpression: public ExpressionAST {
public:
    FunctionCallExpression(std::unique_ptr<ExpressionAST> name, std::vector<std::unique_ptr<ExpressionAST>> arguments)
    : function_(std::move(name))
    , arguments_(std::move(arguments)) {}

    ExpressionType GetExpressionType() override {
        return ExpressionType::kFunctionCallExpression;
    }

    ValuePtr evaluate(Context &context) override {
        std::vector<ValuePtr> result_arguments;
        for (auto& argument : arguments_) {
            result_arguments.push_back(argument->evaluate(context));
        }
        return function_->evaluate(context)->AsFunctionCall(result_arguments, context);
    }

private:
    std::unique_ptr<ExpressionAST> function_;
    std::vector<std::unique_ptr<ExpressionAST>> arguments_;
};