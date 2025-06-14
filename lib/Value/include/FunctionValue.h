#pragma once

#include "ValueInterface.h"

#include "../../AST/Statement.h"
#include "../../executor/Context.h"

class FunctionValue final: public Value {
public:
    FunctionValue(std::unique_ptr<StatementAST> function_body,
                  std::vector<std::string> arguments);

    [[nodiscard]] ValueType GetValueType() const override;
    [[nodiscard]] double AsDouble() const override;
    [[nodiscard]] bool AsBool() const override;
    [[nodiscard]] const std::string& AsString() const override;
    [[nodiscard]] const std::vector<std::unique_ptr<Value>>& AsArray() const override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) const override;

private:
    std::unique_ptr<StatementAST> scope_;
    std::vector<std::string> arguments_;
};