#pragma once

#include "Expression.h"
#include "src/value/Value.h"


namespace itmo_script::ast {

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
