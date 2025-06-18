#pragma once

#include "src/value/headers/ValueInterface.h"

#include "src/AST/Statement.h"
#include "src/executor/Context.h"


class FunctionValue final: public Value {
public:
    FunctionValue(std::unique_ptr<StatementAST> function_body,
                  std::vector<std::string> arguments);

    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) override;

private:
    std::unique_ptr<StatementAST> scope_;
    std::vector<std::string> arguments_;
};