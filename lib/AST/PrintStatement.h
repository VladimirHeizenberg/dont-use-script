#pragma once

#include "Statement.h"
#include "Expression.h"

#include <memory>
#include <iostream>

class PrintStatement: public StatementAST {
public:
    PrintStatement(std::unique_ptr<ExpressionAST> expr)
    : expr_(std::move(expr)) {}

    void execute(Context& context) override {
        context.output() << expr_->evaluate(context);
    }
private:
    std::unique_ptr<ExpressionAST> expr_;
};


class PrintlnStatement: public StatementAST {
public:
    PrintlnStatement(std::unique_ptr<ExpressionAST> expr)
    : expr_(std::move(expr)) {}

    void execute(Context& context) override {
        context.output() << expr_->evaluate(context) << std::endl;
    }
private:
    std::unique_ptr<ExpressionAST> expr_;
};