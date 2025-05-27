#pragma once

#include <string>
#include <stdexcept>

#include "Expression.h"
#include "VariablesTable.h"
#include "Value.h"

class VariableExpression: public ExpressionAST {
public:
    VariableExpression(const std::string& name, VariablesTable& table)
    : name_(name)
    , table_(table) {}

    Value evaluate() override {
        if (!table_.Contains(name_)) throw std::runtime_error("Name '" + name_ + "' is not defined\n");
        return table_.Get(name_);
    }
private:
    std::string name_;
    VariablesTable& table_;
};