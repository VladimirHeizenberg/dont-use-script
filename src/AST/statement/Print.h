#pragma once

#include <memory>
#include <iostream>

#include "src/ast/statement/Statement.h"
#include "src/ast//expression/Expression.h"


namespace itmo_script::ast {

class PrintStmt: public Statement {
public:
    PrintStmt(std::unique_ptr<Expression> expr)
    : expr_(std::move(expr)) {}

    StatementResultProxy Execute(executor::Context& context) override {
        context.output() << expr_->Evaluate(context)->AsString();
        return normal_result();
    }
private:
    std::unique_ptr<Expression> expr_;
};


class PrintlnStmt: public Statement {
public:
    PrintlnStmt(std::unique_ptr<Expression> expr)
    : expr_(std::move(expr)) {}

    StatementResultProxy Execute(executor::Context& context) override {
        context.output() << expr_->Evaluate(context)->AsString() << std::endl;
        return normal_result();
    }
private:
    std::unique_ptr<Expression> expr_;
};

} // namespace itmo_script::ast