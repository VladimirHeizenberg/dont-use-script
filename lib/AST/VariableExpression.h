#pragma once

#include <string>
#include <stdexcept>

#include "Expression.h"
#include "VariablesTable.h"
#include "Value.h"

class VariableExpression: public ExpressionAST {
public:
    VariableExpression(const std::string& name)
    : name_(name) {}

    Value evaluate(Context& context) override {
        if (!context.table().Contains(name_)) throw std::runtime_error("Name '" + name_ + "' is not defined\n");
        return context.table().Get(name_);
    }
private:
    std::string name_;
};