#pragma once

#include <string>

#include "Expression.h"
#include "VariablesTable.h"

class VariableExpression: public ExpressionAST {
public:
    VariableExpression(const std::string& name, VariablesTable& table)
    : name_(name)
    , table_(table) {}

    double evaluate() override {
        return table_.Get(name_);
    }
private:
    std::string name_;
    VariablesTable& table_;
};