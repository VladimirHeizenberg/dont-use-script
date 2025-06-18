#pragma once

#include "src/AST/Statement.h"
#include "src/AST/Expression.h"

class ReturnStatement: public StatementAST {
public:
    ReturnStatement(std::unique_ptr<ExpressionAST> expression)
    : return_expression(std::move(expression)) {}

    StatementResultProxy execute(Context &context) override {
        return {return_expression->evaluate(context), StatementResult::kReturn};
    }
private:
    std::unique_ptr<ExpressionAST> return_expression;
};