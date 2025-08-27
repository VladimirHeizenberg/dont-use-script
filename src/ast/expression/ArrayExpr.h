#pragma once

#include "Expression.h"
#include "value/MakeValue.h"

namespace itmo_script::ast {

class ArrayExpr: public Expression {
public:
    ArrayExpr(std::vector<std::unique_ptr<Expression>> elements)
        : elements_(std::move(elements)) {}

    ExprType GetExpressionType() override {
        return ExprType::kConstExpression;
    }

    value::ValuePtr Evaluate(executor::Context& context) override {
        auto array = value::MakeArray();
        array->AsArray().reserve(elements_.size());
        for (auto& element : elements_) {
            array->AsArray().push_back(element->Evaluate(context));
        }
        return array;
    }
private:
    std::vector<std::unique_ptr<Expression>> elements_;
};

} // namespace itmo_script::ast
