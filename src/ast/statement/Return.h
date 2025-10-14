#pragma once


#include "Statement.h"
#include "ast/expression/Expression.h"
#include "executor/Context.h"

namespace itmo_script::ast {

class ReturnStmt: public Statement {
public:
    ReturnStmt(std::unique_ptr<Expression> expression)
    : return_expression(std::move(expression)) {}

    StatementResultProxy Execute(executor::Context& context) override {
        return {return_expression->Evaluate(context), StatementResult::kReturn};
    }
private:
    std::unique_ptr<Expression> return_expression;
};

} // namespace itmo_script::ast