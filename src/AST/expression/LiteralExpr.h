#pragma once

#include <stdexcept>

#include "Expression.h"
#include "src/value/Value.h"
#include "src/executor/VariablesTable.h"


namespace itmo_script::ast {

class LiteralExpr: public Expression {
public:
    LiteralExpr(std::string name)
    : name_(std::move(name)) {}

    ExprType GetExpressionType() override {
        return ExprType::kLiteralExpression;
    }

    value::ValuePtr Evaluate(executor::Context& context) override {
        if (!context.table().Contains(name_)) {
            throw std::runtime_error("Name '" + name_ + "' is not defined\n");
        }
        return context.table().Get(name_);
    }
private:
    std::string name_;
}; 

} // namespace itmo_script::ast