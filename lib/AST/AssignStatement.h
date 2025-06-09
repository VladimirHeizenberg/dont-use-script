#pragma once

#include <string>
#include <memory>

#include "Expression.h"
#include "Statement.h"
#include "VariablesTable.h"

enum class AssignmentOperationType {
    kAssign,
    kPlusAssign,
    kMinusAssign,
    kMulAssign,
    kDivAssign,
};

class AssignStatement final: public StatementAST {
public:
    AssignStatement(const std::string& name, 
                       std::unique_ptr<ExpressionAST> expr,
                       AssignmentOperationType type)
    : name_(name) {
        if (type == AssignmentOperationType::kAssign) {
            expr_ = std::move(expr);
        } else {
            expr_ = std::make_unique<BinaryExpressionAST>(
                to_operators_table_.at(type), 
                std::make_unique<VariableExpression>(name), 
                std::move(expr)
            );
        }
    }

    void execute(Context& context) override {
        auto result = expr_->evaluate(context);
        context.table().Set(name_, result);
    }
private:
    std::string name_;
    std::unique_ptr<ExpressionAST> expr_;
    const inline static std::unordered_map<AssignmentOperationType, OperationType> to_operators_table_ = {
        {AssignmentOperationType::kPlusAssign,  OperationType::kPlusOp},
        {AssignmentOperationType::kMinusAssign, OperationType::kMinusOp},
        {AssignmentOperationType::kMulAssign,   OperationType::kMulOp},
        {AssignmentOperationType::kDivAssign,   OperationType::kDivOp},
    };
};