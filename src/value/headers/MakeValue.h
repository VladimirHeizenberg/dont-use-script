#pragma once

#include <functional>

#include "src/value/headers/ValueInterface.h"
#include "src/AST/Statement.h"


ValuePtr MakeStringValue(const std::string& value);
ValuePtr MakeDoubleValue(double num);
ValuePtr MakeBoolValue(bool value);
ValuePtr MakeFunctionValue(std::unique_ptr<StatementAST> function_body,
                           std::vector<std::string> arguments);
ValuePtr MakeSystemFunctionValue(std::function<ValuePtr(const std::vector<ValuePtr>&)>);
ValuePtr MakeNullValue();
ValuePtr MakeArrayValue();
ValuePtr MakeArrayValue(std::vector<ValuePtr> values);