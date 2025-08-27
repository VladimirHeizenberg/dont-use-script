#pragma once
#include <iostream>

#include "Expression.h"
#include "value/MakeValue.h"

namespace itmo_script::ast {

class IndexExpr: public Expression {
public:
    IndexExpr(std::unique_ptr<Expression> expression, std::unique_ptr<Expression> index_expression)
    : expression_(std::move(expression))
    , index_expression_(std::move(index_expression)) {}

    ExprType GetExpressionType() override {
        return ExprType::kUnaryExpression;
    }

    value::ValuePtr Evaluate(executor::Context& context) override {
        auto result_of_expr = expression_->Evaluate(context);
        auto index_res = index_expression_->Evaluate(context)->AsDouble();
        if (result_of_expr->GetValueType() == value::ValueType::kArrayValue) {
            auto& array = result_of_expr->AsArray();
            return array.at(index_res < 0 ? array.size() + index_res : index_res);
        }
        if (result_of_expr->GetValueType() == value::ValueType::kStringValue) {
            auto& str = result_of_expr->AsString();
            return value::MakeString(
                std::string{str.at(index_res < 0 ? str.size() + index_res : index_res)}
            );
        }
        throw std::runtime_error("Invalid index expression");
    }
private:
    std::unique_ptr<Expression> expression_;
    std::unique_ptr<Expression> index_expression_;
};

}