#pragma once

#include "src/executor/Context.h"

#include <stdexcept>

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
    // only for Assignable expression
    virtual ValuePtr& GerReference(Context& context) {
        throw std::runtime_error("Value cannot be assignment");
    }
    virtual ~ExpressionAST() = default;
};