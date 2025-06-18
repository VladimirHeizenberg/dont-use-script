#pragma once

#include "src/value/headers/ValueInterface.h"
#include <string>
#include <vector>
#include <memory>


class DoubleValue final : public Value {
public:
    explicit DoubleValue(double num);

    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) override;

private:
    double num_;
    std::string str_value_;
};
