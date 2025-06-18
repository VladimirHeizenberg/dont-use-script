#pragma once

#include "src/AST/Statement.h"
#include <memory>
#include <vector>

class ScopeStatement final: public StatementAST {
public:
    void add(std::unique_ptr<StatementAST> statement) {
        statements_.push_back(std::move(statement));
    }

    StatementResultProxy execute(Context& context) override {
        for (auto& statement: statements_) {
            StatementResultProxy tmp_result = statement->execute(context);
            if (tmp_result.result != StatementResult::kNormal) {
                return tmp_result;
            }
        }
        return normal_result();
    }
private:
    std::vector<std::unique_ptr<StatementAST>> statements_;
};