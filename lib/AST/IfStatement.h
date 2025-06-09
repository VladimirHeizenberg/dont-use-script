#pragma once

#include "Statement.h"
#include "Expression.h"


class IfStatement final: public StatementAST {
public:
    IfStatement(std::unique_ptr<ExpressionAST> expr,
                std::unique_ptr<StatementAST> statement_true,
                std::unique_ptr<StatementAST> statement_false)
    : expr_(std::move(expr))
    , statement_true_(std::move(statement_true))
    , statement_false_(std::move(statement_false)) {}

    void execute(Context& context) override {
        if (expr_->evaluate(context)) {
            statement_true_->execute(context);
        } else {
            statement_false_->execute(context);
        }
    }
private:
    std::unique_ptr<ExpressionAST> expr_;
    std::unique_ptr<StatementAST> statement_true_;
    std::unique_ptr<StatementAST> statement_false_;
};