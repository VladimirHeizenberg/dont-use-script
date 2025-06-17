#pragma once

#include "ValueInterface.h"

class ArrayValue: public Value {
public:
    explicit ArrayValue();
    explicit ArrayValue(std::vector<ValuePtr>);
    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) override;
private:
    std::vector<ValuePtr> array_;
    std::string str_;
};