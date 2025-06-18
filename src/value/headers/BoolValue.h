#pragma once

#include "src/value/headers/ValueInterface.h"


class BoolValue final : public Value {
public:
    explicit BoolValue(bool value);

    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) override;

private:
    bool value_;
    std::string str_value_;
};