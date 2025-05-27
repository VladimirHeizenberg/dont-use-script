#pragma once

#include "Statement.h"
#include "Expression.h"


class IfStatement: public StatementAST {
public:
    IfStatement(std::unique_ptr<ExpressionAST> expr,
                std::unique_ptr<StatementAST> statement_true,
                std::unique_ptr<StatementAST> statement_false)
    : expr_(std::move(expr))
    , statement_true_(std::move(statement_true))
    , statement_false_(std::move(statement_false)) {}

    void execute() override {
        if (expr_->evaluate()) {
            statement_true_->execute();
        } else {
            statement_false_->execute();
        }
    }
private:
    std::unique_ptr<ExpressionAST> expr_;
    std::unique_ptr<StatementAST> statement_true_;
    std::unique_ptr<StatementAST> statement_false_;
};