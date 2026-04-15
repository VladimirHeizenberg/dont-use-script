#pragma once

#include "Expression.h"
#include "value/Value.h"


namespace dont_use_script::ast {

class CnstExpr: public Expression {
public:
    CnstExpr(value::ValuePtr value)
    : value_(std::move(value)) {}

    value::ValuePtr Evaluate(executor::Context&) override {
        return value_;
    }

    ExprType GetExpressionType() override {
        return ExprType::kConstExpression;
    }
private:
    value::ValuePtr value_;
};

}
