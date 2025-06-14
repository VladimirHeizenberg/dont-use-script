#pragma once

#include "Statement.h"
#include "Expression.h"


class WhileStatement final: public StatementAST {
public:
    WhileStatement(std::unique_ptr<ExpressionAST> while_expr,
                   std::unique_ptr<StatementAST> statement)
    : while_expr_(std::move(while_expr))
    , statement_(std::move(statement)) {}

    StatementResultProxy execute(Context& context) override {
        while (while_expr_->evaluate(context)->AsBool()) {
            StatementResultProxy tmp_result = statement_->execute(context);
            if (tmp_result.result == StatementResult::kBreak) {
                break;
            }
            if (tmp_result.result == StatementResult::kReturn) {
                return tmp_result;
            }
        }
        return normal_result();
    }

private:
    std::unique_ptr<ExpressionAST> while_expr_;
    std::unique_ptr<StatementAST> statement_;
};
