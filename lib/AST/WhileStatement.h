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

    void execute(Context& context) {
        while (while_expr_->evaluate(context)) {
            statement_->execute(context);
        }
    }

private:
    std::unique_ptr<ExpressionAST> while_expr_;
    std::unique_ptr<StatementAST> statement_;
};
