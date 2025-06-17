#pragma once

#include "Expression.h"

class IndexExpression final: public ExpressionAST {
public:
    IndexExpression(std::unique_ptr<ExpressionAST> expression, std::unique_ptr<ExpressionAST> index_expression)
    : expression_(std::move(expression))
    , index_expression_(std::move(index_expression)) {}

    ExpressionType GetExpressionType() override {
        return ExpressionType::kUnaryExpression;
    }

    ValuePtr evaluate(Context& context) override {
        auto result_of_expr = expression_->evaluate(context);
        if (result_of_expr->GetValueType() == ValueType::kArrayValue) {
            return result_of_expr->AsArray().at(index_expression_->evaluate(context)->AsDouble());
        }
        if (result_of_expr->GetValueType() == ValueType::kStringValue) {
            return MakeStringValue(
                std::string{result_of_expr->AsString().at(index_expression_->evaluate(context)->AsDouble())}
            );
        }
        throw std::runtime_error("Invalid index expression");
    }
private:
    std::unique_ptr<ExpressionAST> expression_;
    std::unique_ptr<ExpressionAST> index_expression_;
};