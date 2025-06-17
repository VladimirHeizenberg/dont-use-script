#pragma once

#include <memory>
#include <vector>

#include "AST/Statement.h"

class StatementSource {
public:
    virtual std::unique_ptr<StatementAST>& Get() = 0;
    [[nodiscard]] virtual bool eof() const = 0;
    virtual ~StatementSource() = default;
};

class VectorStatementSource: public StatementSource {
public:
    VectorStatementSource(std::vector<std::unique_ptr<StatementAST>>&& statements)
    : statements_(std::move(statements)) 
    , index_(0) {}

    std::unique_ptr<StatementAST>& Get() override {
        return statements_[index_++];
    }

    [[nodiscard]] bool eof() const override {
        return index_ == statements_.size();
    }
private:
    std::vector<std::unique_ptr<StatementAST>> statements_;
    int index_;
};