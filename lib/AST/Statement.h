#pragma once

class StatementAST {
public:
    virtual void execute() = 0;
    virtual ~StatementAST() = default;
};

class EmptyStatement: public StatementAST {
    void execute() override {}
};