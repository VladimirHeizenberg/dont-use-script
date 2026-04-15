#pragma once

#include <memory>
#include <vector>

#include "Statement.h"


namespace dont_use_script::ast {

class ScopeStmt: public Statement {
public:
    void add(std::unique_ptr<Statement> statement) {
        statements_.push_back(std::move(statement));
    }

    StatementResultProxy Execute(executor::Context& context) override {
        for (auto& statement: statements_) {
            StatementResultProxy tmp_result = statement->Execute(context);
            if (tmp_result.result != StatementResult::kNormal) {
                return tmp_result;
            }
        }
        return normal_result();
    }
private:
    std::vector<std::unique_ptr<Statement>> statements_;
};

} // namespace dont_use_script::ast