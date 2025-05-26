#pragma once

class ExpressionAST {
public:
    virtual double evaluate() = 0;
    virtual ~ExpressionAST() = default;
};