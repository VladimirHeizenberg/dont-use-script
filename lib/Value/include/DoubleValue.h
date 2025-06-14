#pragma once

#include "ValueInterface.h"
#include <string>
#include <vector>
#include <memory>

class DoubleValue final : public Value {
public:
    explicit DoubleValue(double num);

    [[nodiscard]] ValueType GetValueType() const override;
    [[nodiscard]] double AsDouble() const override;
    [[nodiscard]] bool AsBool() const override;
    [[nodiscard]] const std::string& AsString() const override;
    [[nodiscard]] const std::vector<std::unique_ptr<Value>>& AsArray() const override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) const override;

private:
    double num_;
    std::string str_value_;
};
