#pragma once

#include "Statement.h"
#include "FunctionCallExpression.h"


class FunctionCallStatement: public StatementAST {
public:
    FunctionCallStatement(std::unique_ptr<ExpressionAST> expression)
    : expression_(std::move(expression)) {}

    StatementResultProxy execute(Context &context) override {
        return {expression_->evaluate(context), StatementResult::kNormal};
    }

private:
    std::unique_ptr<ExpressionAST> expression_;
};