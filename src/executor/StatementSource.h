#pragma once

#include <memory>
#include <vector>

#include "src/ast/statement/Statement.h"

namespace itmo_script::executor {

class StatementSource {
public:
    virtual std::unique_ptr<ast::Statement>& Get() = 0;
    [[nodiscard]] virtual bool eof() const = 0;
    virtual ~StatementSource() = default;
};

class VectorStatementSource: public StatementSource {
public:
    VectorStatementSource(std::vector<std::unique_ptr<ast::Statement>>&& statements)
    : statements_(std::move(statements)) 
    , index_(0) {}

    std::unique_ptr<ast::Statement>& Get() override {
        return statements_[index_++];
    }

    [[nodiscard]] bool eof() const override {
        return index_ == statements_.size();
    }
private:
    std::vector<std::unique_ptr<ast::Statement>> statements_;
    int index_;
};

} // namespace itmo_script::executor