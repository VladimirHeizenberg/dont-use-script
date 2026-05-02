#pragma once

#include "Statement.h"
#include "ast/expression/Expression.h"

namespace dont_use_script::ast {

class ExprStmt: public Statement {
public:
    ExprStmt(std::unique_ptr<ast::Expression> expression)
    : expression_(std::move(expression)) {}

    StatementResultProxy Execute(executor::Context &context) override {
        return {expression_->Evaluate(context), StatementResult::kNormal};
    }

private:
    std::unique_ptr<Expression> expression_;
};

}