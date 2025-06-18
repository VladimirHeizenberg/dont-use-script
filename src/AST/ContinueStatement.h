#pragma once

#include "src/AST/Statement.h"

class ContinueStatement: public StatementAST {
public:
    StatementResultProxy execute(Context& context) override {
        return {nullptr, StatementResult::kContinue};
    }
};