#pragma once

#include "src/AST/Statement.h"
#include "src/AST/Expression.h"

#include <memory>
#include <iostream>

class PrintStatement final: public StatementAST {
public:
    PrintStatement(std::unique_ptr<ExpressionAST> expr)
    : expr_(std::move(expr)) {}

    StatementResultProxy execute(Context& context) override {
        context.output() << expr_->evaluate(context)->AsString();
        return normal_result();
    }
private:
    std::unique_ptr<ExpressionAST> expr_;
};


class PrintlnStatement: public StatementAST {
public:
    PrintlnStatement(std::unique_ptr<ExpressionAST> expr)
    : expr_(std::move(expr)) {}

    StatementResultProxy execute(Context& context) override {
        context.output() << expr_->evaluate(context)->AsString() << std::endl;
        return normal_result();
    }
private:
    std::unique_ptr<ExpressionAST> expr_;
};