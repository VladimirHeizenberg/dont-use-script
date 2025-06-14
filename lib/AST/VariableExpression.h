#pragma once

#include <string>
#include <stdexcept>

#include "Expression.h"
#include "VariablesTable.h"

class VariableExpression final: public ExpressionAST {
public:
    VariableExpression(std::string name)
    : name_(std::move(name)) {}

    ValuePtr evaluate(Context& context) override {
        if (!context.table().Contains(name_)) throw std::runtime_error("Name '" + name_ + "' is not defined\n");
        return context.table().Get(name_);
    }
private:
    std::string name_;
};