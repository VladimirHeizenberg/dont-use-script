#pragma once

#include "Statement.h"
#include "Expression.h"
#include "Value.h"


class WhileStatement: public StatementAST {
public:
    WhileStatement(std::unique_ptr<ExpressionAST> while_expr,
                   std::unique_ptr<StatementAST> statement)
    : while_expr_(std::move(while_expr))
    , statement_(std::move(statement)) {}

    void execute() {
        while (while_expr_->evaluate()) {
            statement_->execute();
        }
    }

private:
    std::unique_ptr<ExpressionAST> while_expr_;
    std::unique_ptr<StatementAST> statement_;
};
