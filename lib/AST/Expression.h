#pragma once

#include "Value.h"

class ExpressionAST {
public:
    virtual Value evaluate() = 0;
    virtual ~ExpressionAST() = default;
};