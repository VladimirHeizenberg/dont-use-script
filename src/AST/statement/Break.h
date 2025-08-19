#pragma once

#include "Statement.h"


namespace itmo_script::ast {

class BreakStmt: public Statement {
public:
    StatementResultProxy Execute(executor::Context& context) override {
        return {nullptr, StatementResult::kBreak};
    }
};

} // namespace itmo_script::ast