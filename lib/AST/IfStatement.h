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

    StatementResultProxy execute(Context& context) override {
        auto execution_lambda = [&]() -> StatementResultProxy {
            if (expr_->evaluate(context)->AsBool()) {
                return statement_true_->execute(context);
            }
            return statement_false_->execute(context);
        };

        StatementResultProxy tmp_result = execution_lambda();
        if (tmp_result.result != StatementResult::kNormal) {
            return tmp_result;
        }
        return normal_result();
    }
private:
    std::unique_ptr<ExpressionAST> expr_;
    std::unique_ptr<StatementAST> statement_true_;
    std::unique_ptr<StatementAST> statement_false_;
};