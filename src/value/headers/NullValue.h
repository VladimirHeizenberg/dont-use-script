#pragma once

#include "src/value/headers/ValueInterface.h"


class NullValue final : public Value {
public:
    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) override;
private:
    std::string str_;
    std::vector<ValuePtr> array_;
};