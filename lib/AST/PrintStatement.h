#pragma once

#include "Statement.h"
#include "Expression.h"

#include <memory>
#include <iostream>

class PrintStatement: public StatementAST {
public:
    PrintStatement(std::unique_ptr<ExpressionAST> expr)
    : expr_(std::move(expr)) {}

    void execute() override {
        std::cout << expr_->evaluate();
    }
private:
    std::unique_ptr<ExpressionAST> expr_;
};


class PrintlnStatement: public StatementAST {
public:
    PrintlnStatement(std::unique_ptr<ExpressionAST> expr)
    : expr_(std::move(expr)) {}

    void execute() override {
        std::cout << expr_->evaluate() << std::endl;
    }
private:
    std::unique_ptr<ExpressionAST> expr_;
};