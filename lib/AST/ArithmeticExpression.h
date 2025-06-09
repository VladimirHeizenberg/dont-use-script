#pragma once
#include "Expression.h"

#include <memory>
#include <stdexcept>

enum class OperationType {
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

class ConstExpressionAST final: public ExpressionAST {
public:
    ConstExpressionAST(ValuePtr value)
    : value_(std::move(value)) {}

    ValuePtr evaluate(Context& context) override {
        return value_;
    }
private:
    ValuePtr value_;
};

class UnaryExpressionAST final: public ExpressionAST {
public:
    UnaryExpressionAST(OperationType operation, 
                       std::unique_ptr<ExpressionAST> expr)
    : operation_(operation)
    , expr_(std::move(expr)) {}

    ValuePtr evaluate(Context& context) override {
        ValuePtr result = expr_->evaluate(context);
        switch (operation_) {
        case OperationType::kNoOp:
            return result;
        case OperationType::kPlusOp: 
            if (result->GetValueType() == ValueType::kDoubleValue)
                return result;
            throw std::runtime_error("Wrong type for unary expression +\n");
        case OperationType::kMinusOp: 
            if (result->GetValueType() == ValueType::kDoubleValue) {
                return std::make_shared<DoubleValue>(-result->AsDouble());
            }
            throw std::runtime_error("Wrong type for unary expression +\n");
        // case OperationType::kLogicalNot:
        //     return !result;
        default:
            throw std::runtime_error("Wrong unary operator");
        }
    }
private:
    OperationType operation_;
    std::unique_ptr<ExpressionAST> expr_;
};

class BinaryExpressionAST final: public ExpressionAST {
public:
    BinaryExpressionAST(OperationType operation, 
                     std::unique_ptr<ExpressionAST> lhs, 
                     std::unique_ptr<ExpressionAST> rhs)
    : operation_(operation)
    , lhs_(std::move(lhs))
    , rhs_(std::move(rhs)) {}

    ValuePtr evaluate(Context& context) override {
        const ValuePtr lhs = lhs_->evaluate(context);
        const ValuePtr rhs = rhs_->evaluate(context);
        switch (operation_)
        {
        case OperationType::kPlusOp: return Add(lhs, rhs);
        case OperationType::kMinusOp: return Subtract(lhs, rhs);
        case OperationType::kMulOp: return Multiply(lhs, rhs);
        case OperationType::kDivOp: return Divide(lhs, rhs);
        // case OperationType::kLogicalAnd: return lhs & rhs;
        // case OperationType::kLogicalOr: return lhs | rhs;
        // case OperationType::kLess: return lhs < rhs;
        // case OperationType::kGreater: return lhs > rhs;
        // case OperationType::kLessOrEqual: return lhs <= rhs;
        // case OperationType::kGreaterOrEqual: return lhs >= rhs;
        // case OperationType::kEqual: return lhs == rhs;
        // case OperationType::kNotEqual: return lhs != rhs;
        default: throw std::runtime_error("Unknow operation");
        }
    }

private:
    OperationType operation_;
    std::unique_ptr<ExpressionAST> lhs_;
    std::unique_ptr<ExpressionAST> rhs_;
};