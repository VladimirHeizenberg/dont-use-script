#pragma once

#include "Statement.h"
#include <memory>
#include <vector>

class ScopeStatement final: public StatementAST {
public:
    void add(std::unique_ptr<StatementAST> statement) {
        statements_.push_back(std::move(statement));
    }

    void execute(Context& context) override {
        for (auto& statement: statements_) {
            statement->execute(context);
        }
    }
private:
    std::vector<std::unique_ptr<StatementAST>> statements_;
};