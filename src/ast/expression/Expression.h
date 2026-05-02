#pragma once

#include "value/Value.h"
#include "executor/Context.h"

namespace dont_use_script::ast {

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

} // namespace dont_use_script::ast