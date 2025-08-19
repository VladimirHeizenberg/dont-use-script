#pragma once

#include "Statement.h"
#include "src/ast/expression/Expression.h"


namespace itmo_script::ast {

class IfStmt final: public Statement {
public:
    IfStmt(std::unique_ptr<Expression> expr,
                std::unique_ptr<Statement> statement_true,
                std::unique_ptr<Statement> statement_false)
    : expr_(std::move(expr))
    , statement_true_(std::move(statement_true))
    , statement_false_(std::move(statement_false)) {}

    StatementResultProxy Execute(executor::Context& context) override {
        auto execution_lambda = [&]() -> StatementResultProxy {
            if (expr_->Evaluate(context)->AsBool()) {
                return statement_true_->Execute(context);
            }
            return statement_false_->Execute(context);
        };

        StatementResultProxy tmp_result = execution_lambda();
        if (tmp_result.result != StatementResult::kNormal) {
            return tmp_result;
        }
        return normal_result();
    }
private:
    std::unique_ptr<Expression> expr_;
    std::unique_ptr<Statement> statement_true_;
    std::unique_ptr<Statement> statement_false_;
};

} // namespace itmo_script::ast