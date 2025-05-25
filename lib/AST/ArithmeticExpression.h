#pragma once
#include "Expression.h"

#include <memory>
#include <stdexcept>

enum OperationType {
    kPlusOp,
    kMinusOp,
    kMulOp,
    kDivOp,
};

class ConstExpressionAST : public ExpressionAST {
public:
    ConstExpressionAST(double value)
    : value_(value) {}

    double evaluate() override {
        return value_;
    }
private:
    double value_;
};

class UnaryExpressionAST : public ExpressionAST {
public:
    UnaryExpressionAST(OperationType operation, 
                     std::unique_ptr<ExpressionAST> expr)
    : operation_(operation)
    , expr_(std::move(expr)) {}

    double evaluate() override {
        switch (operation_) {
        case OperationType::kPlusOp: return expr_->evaluate();
        case OperationType::kMinusOp: return (-expr_->evaluate());
        }
        throw std::runtime_error("Wrong unary operator");
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

    double evaluate() override {
        switch (operation_)
        {
        case OperationType::kPlusOp: return lhs_->evaluate() + rhs_->evaluate();
        case OperationType::kMinusOp: return lhs_->evaluate() - rhs_->evaluate();
        case OperationType::kMulOp: return lhs_->evaluate() * rhs_->evaluate();
        case OperationType::kDivOp: return lhs_->evaluate() / rhs_->evaluate();
        }
        throw std::runtime_error("Unknow operation");
    }

private:
    OperationType operation_;
    std::unique_ptr<ExpressionAST> lhs_;
    std::unique_ptr<ExpressionAST> rhs_;
};