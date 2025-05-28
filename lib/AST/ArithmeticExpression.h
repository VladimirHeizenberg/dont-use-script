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

    Value evaluate() override {
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

    Value evaluate() override {
        Value result = expr_->evaluate();
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

    Value evaluate() override {
        const Value lhs = lhs_->evaluate();
        const Value rhs = rhs_->evaluate();
        switch (operation_)
        {
        case OperationType::kPlusOp: return lhs_->evaluate() + rhs_->evaluate();
        case OperationType::kMinusOp: return lhs_->evaluate() - rhs_->evaluate();
        case OperationType::kMulOp: return lhs_->evaluate() * rhs_->evaluate();
        case OperationType::kDivOp: return lhs_->evaluate() / rhs_->evaluate();
        case OperationType::kLogicalAnd: return lhs_->evaluate() & rhs_->evaluate();
        case OperationType::kLogicalOr: return lhs_->evaluate() | rhs_->evaluate();
        case OperationType::kLess: return lhs_->evaluate() < rhs_->evaluate();
        case OperationType::kGreater: return lhs_->evaluate() > rhs_->evaluate();
        case OperationType::kLessOrEqual: return lhs_->evaluate() <= rhs_->evaluate();
        case OperationType::kGreaterOrEqual: return lhs_->evaluate() >= rhs_->evaluate();
        case OperationType::kEqual: return lhs_->evaluate() == rhs_->evaluate();
        case OperationType::kNotEqual: return lhs_->evaluate() != rhs_->evaluate();
        default: throw std::runtime_error("Unknow operation");
        }
    }

private:
    OperationType operation_;
    std::unique_ptr<ExpressionAST> lhs_;
    std::unique_ptr<ExpressionAST> rhs_;
};