#pragma once

#include <string>
#include <memory>

#include "Expression.h"
#include "Statement.h"
#include "VariablesTable.h"

class AssignStatementAST: public StatementAST {
public:
    AssignStatementAST(const std::string& name, 
                       std::unique_ptr<ExpressionAST> expr,
                       VariablesTable& table)
    : name_(name)
    , expr_(std::move(expr))
    , table_(table) {}

    void execute() override {
        auto result = expr_->evaluate();
        table_.Set(name_, result);
    }
private:
    std::string name_;
    std::unique_ptr<ExpressionAST> expr_;
    VariablesTable& table_;
};