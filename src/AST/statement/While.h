#pragma once

#include "Statement.h"
#include "src/ast/expression/Expression.h"

namespace itmo_script::ast {

class WhileStmt: public Statement {
public:
    WhileStmt(std::unique_ptr<Expression> while_expr,
          std::unique_ptr<Statement> statement)
    : while_expr_(std::move(while_expr))
    , statement_(std::move(statement)) {}

    StatementResultProxy Execute(executor::Context& context) override {
        while (while_expr_->Evaluate(context)->AsBool()) {
            StatementResultProxy tmp_result = statement_->Execute(context);
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
    std::unique_ptr<Expression> while_expr_;
    std::unique_ptr<Statement> statement_;
};

} // namespace itmo_script::ast