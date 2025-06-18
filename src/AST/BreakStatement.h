#pragma once

#include "src/AST/Statement.h"

class BreakStatement: public StatementAST {
public:
    StatementResultProxy execute(Context& context) override {
        return {nullptr, StatementResult::kBreak};
    }
};