#pragma once

#include "executor/Context.h"

enum class StatementResult {
    kNormal,
    kBreak,
    kContinue,
    kReturn,
};

struct StatementResultProxy {
    StatementResultProxy(ValuePtr value_ptr, StatementResult statement_result)
    : value(std::move(value_ptr))
    , result(statement_result) {}

    ValuePtr value;
    StatementResult result;
};

inline StatementResultProxy normal_result() {
    return {nullptr, StatementResult::kNormal};
}

class StatementAST {
public:
    virtual StatementResultProxy execute(Context& context) = 0;
    virtual ~StatementAST() = default;
};

class EmptyStatement final: public StatementAST {
    StatementResultProxy execute(Context& context) override {
        return StatementResultProxy{nullptr, StatementResult::kNormal};
    }
};