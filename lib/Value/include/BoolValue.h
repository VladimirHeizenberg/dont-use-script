#pragma once

#include "ValueInterface.h"

class BoolValue final : public Value {
public:
    explicit BoolValue(bool value);

    [[nodiscard]] ValueType GetValueType() const override;
    [[nodiscard]] bool AsBool() const override;
    [[nodiscard]] double AsDouble() const override;
    [[nodiscard]] const std::string& AsString() const override;
    [[nodiscard]] const std::vector<std::unique_ptr<Value>>& AsArray() const override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) const override;

private:
    bool value_;
    std::string str_value_;
};