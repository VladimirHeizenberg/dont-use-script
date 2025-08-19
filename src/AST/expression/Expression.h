#pragma once

#include "src/value/Value.h"
#include "src/executor/Context.h"

namespace itmo_script::ast {

enum class ExprType {
    kBinaryExpression,
    kConstExpression,
    kUnaryExpression,
    kLiteralExpression,
    kFunctionCallExpression,
};

/* interface */
class Expression {
public:
    virtual value::ValuePtr Evaluate(executor::Context&) = 0;
    virtual ExprType GetExpressionType() = 0;
    virtual ~Expression() = default;
};

} // namespace itmo_script::ast