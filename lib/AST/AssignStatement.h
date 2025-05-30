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

class AssignStatement: public StatementAST {
public:
    AssignStatement(const std::string& name, 
                       std::unique_ptr<ExpressionAST> expr,
                       VariablesTable& table,
                       AssignmentOperationType type)
    : name_(name)
    , table_(table) {
        if (type == AssignmentOperationType::kAssign) {
            expr_ = std::move(expr);
        } else {
            expr_ = std::make_unique<BinaryExpressionAST>(
                to_operators_table_.at(type), 
                std::make_unique<VariableExpression>(name, table), 
                std::move(expr)
            );
        }
    }

    void execute() override {
        auto result = expr_->evaluate();
        table_.Set(name_, result);
    }
private:
    std::string name_;
    std::unique_ptr<ExpressionAST> expr_;
    VariablesTable& table_;
    const inline static std::unordered_map<AssignmentOperationType, OperationType> to_operators_table_ = {
        {AssignmentOperationType::kPlusAssign,  OperationType::kPlusOp},
        {AssignmentOperationType::kMinusAssign, OperationType::kMinusOp},
        {AssignmentOperationType::kMulAssign,   OperationType::kMulOp},
        {AssignmentOperationType::kDivAssign,   OperationType::kDivOp},
    };
};