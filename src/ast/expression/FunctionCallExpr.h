#pragma once

#include "Expression.h"


namespace itmo_script::ast {

class FunctionCallExpr: public Expression {
public:
    FunctionCallExpr(std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> arguments)
    : function_(std::move(name))
    , arguments_(std::move(arguments)) {}

    ExprType GetExpressionType() override {
        return ExprType::kFunctionCallExpression;
    }

    value::ValuePtr Evaluate(executor::Context& context) override {
        std::vector<value::ValuePtr> result_arguments;
        for (auto& argument : arguments_) {
            result_arguments.push_back(argument->Evaluate(context));
        }
        return function_->Evaluate(context)->AsFunctionCall(result_arguments, context);
    }

private:
    std::unique_ptr<Expression> function_;
    std::vector<std::unique_ptr<Expression>> arguments_;
};

} // namespace itmo_script