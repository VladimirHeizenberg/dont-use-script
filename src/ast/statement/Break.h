#pragma once

#include "Statement.h"


namespace dont_use_script::ast {

class BreakStmt: public Statement {
public:
    StatementResultProxy Execute(executor::Context& context) override {
        return {nullptr, StatementResult::kBreak};
    }
};

} // namespace dont_use_script::ast