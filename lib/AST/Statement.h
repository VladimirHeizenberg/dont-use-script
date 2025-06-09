#pragma once

#include "executor/Context.h"

class StatementAST {
public:
    virtual void execute(Context& context) = 0;
    virtual ~StatementAST() = default;
};

class EmptyStatement final: public StatementAST {
    void execute(Context& context) override {}
};