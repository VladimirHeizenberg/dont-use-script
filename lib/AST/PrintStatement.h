#pragma once

#include "Statement.h"
#include "Expression.h"

#include <memory>
#include <iostream>

class PrintStatement: public StatementAST {
public:
    PrintStatement(std::unique_ptr<ExpressionAST> expr, 
                   std::ostream& out)
    : expr_(std::move(expr))
    , out_(out) {}

    void execute() override {
        out_ << expr_->evaluate();
    }
private:
    std::ostream& out_;
    std::unique_ptr<ExpressionAST> expr_;
};


class PrintlnStatement: public StatementAST {
public:
    PrintlnStatement(std::unique_ptr<ExpressionAST> expr,
                     std::ostream& out)
    : expr_(std::move(expr))
    , out_(out) {}

    void execute() override {
        out_ << expr_->evaluate() << std::endl;
    }
private:
    std::ostream& out_;
    std::unique_ptr<ExpressionAST> expr_;
};