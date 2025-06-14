#pragma once

#include "ValueInterface.h"
#include "../../AST/Statement.h"

ValuePtr MakeStringValue(const std::string& value);
ValuePtr MakeDoubleValue(double num);
ValuePtr MakeBoolValue(bool value);
ValuePtr MakeFunctionValue(std::unique_ptr<StatementAST> function_body,
                           std::vector<std::string> arguments);