#pragma once

#include "Value.h"
#include "executor/Context.h"

class ExpressionAST {
public:
    virtual Value evaluate(Context& context) = 0;
    virtual ~ExpressionAST() = default;
};