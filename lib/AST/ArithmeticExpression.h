#pragma once
#include "Expression.h"

#include <memory>
#include <stdexcept>

enum OperationType {
    kPlusOp,
    kMinusOp,
    kMulOp,
    kDivOp,
    kNoOp,

    kLogicalAnd,
    kLogicalOr,
    kLogicalNot,
    kEqual,
    kNotEqual,
    kLess,
    kGreater,
    kLessOrEqual,
    kGreaterOrEqual,
};

class ConstExpressionAST : public ExpressionAST {
public:
    ConstExpressionAST(const Value& value)
    : value_(value) {}

    Value evaluate(Context& context) override {
        return value_;
    }
private:
    Value value_;
};

class UnaryExpressionAST : public ExpressionAST {
public:
    UnaryExpressionAST(OperationType operation, 
                       std::unique_ptr<ExpressionAST> expr)
    : operation_(operation)
    , expr_(std::move(expr)) {}

    Value evaluate(Context& context) override {
        Value result = expr_->evaluate(context);
        switch (operation_) {
        case OperationType::kNoOp:
            return result;
        case OperationType::kPlusOp: 
            if (result.Is<double>())
                return result;
            throw std::runtime_error("Wrong type for unary expression +\n");
        case OperationType::kMinusOp: 
            if (result.Is<double>()) {
                return -result.As<double>();
            }
            throw std::runtime_error("Wrong type for unary expression +\n");
        case OperationType::kLogicalNot:
            return !result;
        default:
            throw std::runtime_error("Wrong unary operator");
        }
    }
private:
    OperationType operation_;
    std::unique_ptr<ExpressionAST> expr_;
};

class BinaryExpressionAST : public ExpressionAST {
public:
    BinaryExpressionAST(OperationType operation, 
                     std::unique_ptr<ExpressionAST> lhs, 
                     std::unique_ptr<ExpressionAST> rhs)
    : operation_(operation)
    , lhs_(std::move(lhs))
    , rhs_(std::move(rhs)) {}

    Value evaluate(Context& context) override {
        const Value lhs = lhs_->evaluate(context);
        const Value rhs = rhs_->evaluate(context);
        switch (operation_)
        {
        case OperationType::kPlusOp: return lhs + rhs;
        case OperationType::kMinusOp: return lhs - rhs;
        case OperationType::kMulOp: return lhs * rhs;
        case OperationType::kDivOp: return lhs / rhs;
        case OperationType::kLogicalAnd: return lhs & rhs;
        case OperationType::kLogicalOr: return lhs | rhs;
        case OperationType::kLess: return lhs < rhs;
        case OperationType::kGreater: return lhs > rhs;
        case OperationType::kLessOrEqual: return lhs <= rhs;
        case OperationType::kGreaterOrEqual: return lhs >= rhs;
        case OperationType::kEqual: return lhs == rhs;
        case OperationType::kNotEqual: return lhs != rhs;
        default: throw std::runtime_error("Unknow operation");
        }
    }

private:
    OperationType operation_;
    std::unique_ptr<ExpressionAST> lhs_;
    std::unique_ptr<ExpressionAST> rhs_;
};