#pragma once

#include <functional>

#include "Value.h"
#include "ast/statement/Statement.h"

namespace dont_use_script::value {

ValuePtr MakeString(const std::string& value);
ValuePtr MakeDouble(double num);
ValuePtr MakeBool(bool value);
ValuePtr MakeFunction(std::unique_ptr<ast::Statement> function_body,
                      std::vector<std::string> arguments);
ValuePtr MakeSystemFunction(std::function<ValuePtr(const std::vector<ValuePtr>&)>);
ValuePtr MakeNull();
ValuePtr MakeArray();
ValuePtr MakeArray(std::vector<ValuePtr> values);

}