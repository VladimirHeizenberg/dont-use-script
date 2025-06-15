#pragma once

#include "ValueInterface.h"

class NullValue final : public Value {
public:
    [[nodiscard]] ValueType GetValueType() const override;
    [[nodiscard]] bool AsBool() const override;
    [[nodiscard]] double AsDouble() const override;
    [[nodiscard]] const std::string& AsString() const override;
    [[nodiscard]] const std::vector<std::unique_ptr<Value>>& AsArray() const override;
    [[nodiscard]] ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) const override;
private:
    std::string str_;
    std::vector<std::unique_ptr<Value>> array_;
};