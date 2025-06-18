#pragma once
#include <iostream>
#include "src/AST/Expression.h"

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
        auto index_res = index_expression_->evaluate(context)->AsDouble();
        if (result_of_expr->GetValueType() == ValueType::kArrayValue) {
            auto& array = result_of_expr->AsArray();
            return array.at(index_res < 0 ? array.size() + index_res : index_res);
        }
        if (result_of_expr->GetValueType() == ValueType::kStringValue) {
            auto& str = result_of_expr->AsString();
            return MakeStringValue(
                std::string{str.at(index_res < 0 ? str.size() + index_res : index_res)}
            );
        }
        throw std::runtime_error("Invalid index expression");
    }
private:
    std::unique_ptr<ExpressionAST> expression_;
    std::unique_ptr<ExpressionAST> index_expression_;
};