#pragma once

#include <memory>

#include "src/executor/Context.h"

namespace itmo_script::value {
    class Value;
    using ValuePtr = std::shared_ptr<Value>;
}

namespace itmo_script::ast {

enum class StatementResult {
    kNormal,
    kBreak,
    kContinue,
    kReturn,
};


struct StatementResultProxy {
    StatementResultProxy(value::ValuePtr value_ptr, StatementResult statement_result)
    : value(std::move(value_ptr))
    , result(statement_result) {}

    value::ValuePtr value;
    StatementResult result;
};


inline StatementResultProxy normal_result() {
    return {nullptr, StatementResult::kNormal};
}


/* interface */
class Statement {
public:
    virtual StatementResultProxy Execute(executor::Context&) = 0;
    virtual ~Statement() = default;
};

class EmptyStmt: public Statement {
    StatementResultProxy Execute(executor::Context& context) override {
        return StatementResultProxy{nullptr, StatementResult::kNormal};
    }
};

} // namespace itmo_script::ast