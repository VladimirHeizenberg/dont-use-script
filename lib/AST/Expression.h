#pragma once

#include "../Value/include/Value.h"
#include "executor/Context.h"

class ExpressionAST {
public:
    virtual ValuePtr evaluate(Context& context) = 0;
    virtual ~ExpressionAST() = default;
};