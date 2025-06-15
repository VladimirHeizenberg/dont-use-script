#pragma once

#include "../Value/include/Value.h"
#include "executor/Context.h"

enum class ExpressionType {
    kBinaryExpression,
    kConstExpression,
    kUnaryExpression,
    kVariableExpression,
    kFunctionCallExpression,
};

inline std::string to_string(ExpressionType type) {
    switch (type) {
        case ExpressionType::kBinaryExpression:
            return "BinaryExpression";
        case ExpressionType::kConstExpression:
            return "ConstExpression";
        case ExpressionType::kUnaryExpression:
            return "UnaryExpression";
        case ExpressionType::kVariableExpression:
            return "VariableExpression";
        case ExpressionType::kFunctionCallExpression:
            return "FunctionCallExpression";
        default:
            return "UnknownExpressionType";
    }
}


class ExpressionAST {
public:
    virtual ValuePtr evaluate(Context& context) = 0;
    virtual ExpressionType GetExpressionType() = 0;
    virtual ~ExpressionAST() = default;
};