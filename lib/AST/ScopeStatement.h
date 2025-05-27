#pragma once

#include "Statement.h"
#include <memory>
#include <vector>

class ScopeStatement: public StatementAST {
public:
    void add(std::unique_ptr<StatementAST> statement) {
        statements_.push_back(std::move(statement));
    }

    void execute() override {
        for (auto& statement: statements_) {
            statement->execute();
        }
    }
private:
    std::vector<std::unique_ptr<StatementAST>> statements_;
};