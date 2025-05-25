#pragma once
#include "Expression.h"

#include <memory>

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

class BinaryExpressionAST : public ExpressionAST {
public:
    BinaryExpressionAST(OperationType operation, 
                     std::unique_ptr<ExpressionAST> lhs, 
                     std::unique_ptr<ExpressionAST> rhs)
    : operation_(operation)
    , expr1_(std::move(lhs))
    , expr2_(std::move(rhs)) {}

    double evaluate() override {
        switch (operation_)
        {
        case OperationType::kPlusOp: return expr1_->evaluate() + expr2_->evaluate();
        case OperationType::kMinusOp: return expr1_->evaluate() - expr2_->evaluate();
        case OperationType::kMulOp: return expr1_->evaluate() * expr2_->evaluate();
        case OperationType::kDivOp: return expr1_->evaluate() / expr2_->evaluate();
        }
    }

private:
    std::unique_ptr<ExpressionAST> expr1_, expr2_;
    OperationType operation_;
};