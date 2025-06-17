#pragma once

#include "ValueInterface.h"
#include <string>
#include <vector>
#include <memory>

class StringValue final : public Value {
public:
    explicit StringValue(const std::string& value);

    [[nodiscard]] ValueType GetValueType() override;
    [[nodiscard]] bool AsBool() override;
    [[nodiscard]] std::string& AsString() override;
    [[nodiscard]] std::vector<ValuePtr>& AsArray() override;
    [[nodiscard]] double AsDouble() override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) override;

private:
    std::string value_;
};