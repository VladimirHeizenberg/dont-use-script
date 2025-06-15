#pragma once
#include "Expression.h"

#include <memory>
#include <stdexcept>

enum class OperationType {
    kPlusOp,
    kMinusOp,
    kMulOp,
    kDivOp,
    kRemainderOp,
    kPowerOp,
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

    ExpressionType GetExpressionType() override {
        return ExpressionType::kConstExpression;
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

    ExpressionType GetExpressionType() override {
        return ExpressionType::kUnaryExpression;
    }

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
                return MakeDoubleValue(-result->AsDouble());
            }
            throw std::runtime_error("Wrong type for unary expression -\n");
        case OperationType::kLogicalNot:
            return LogicalNot(result);
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

    ExpressionType GetExpressionType() override {
        return ExpressionType::kBinaryExpression;
    }

    ValuePtr evaluate(Context& context) override {
        const ValuePtr lhs = lhs_->evaluate(context);
        const ValuePtr rhs = rhs_->evaluate(context);
        switch (operation_)
        {
        case OperationType::kPlusOp: return Add(lhs, rhs);
        case OperationType::kMinusOp: return Subtract(lhs, rhs);
        case OperationType::kMulOp: return Multiply(lhs, rhs);
        case OperationType::kDivOp: return Divide(lhs, rhs);
        case OperationType::kRemainderOp: return Remainder(lhs, rhs);
        case OperationType::kLogicalAnd: return LogicalAnd(lhs, rhs);
        case OperationType::kLogicalOr: return LogicalOr(lhs, rhs);
        case OperationType::kLess: return Less(lhs, rhs);
        case OperationType::kGreater: return Greater(lhs, rhs);
        case OperationType::kLessOrEqual: return LessOrEqual(lhs, rhs);
        case OperationType::kGreaterOrEqual: return GreaterOrEqual(lhs, rhs);
        case OperationType::kEqual: return Equals(lhs, rhs);
        case OperationType::kNotEqual: return NotEquals(lhs, rhs);
        default: throw std::runtime_error("Unknown operation");
        }
    }

private:
    OperationType operation_;
    std::unique_ptr<ExpressionAST> lhs_;
    std::unique_ptr<ExpressionAST> rhs_;
};