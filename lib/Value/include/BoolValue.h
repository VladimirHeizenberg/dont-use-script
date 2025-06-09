#pragma once

#include "ValueInterface.h"

class BoolValue final : public Value {
public:
    explicit BoolValue(bool value);

    [[nodiscard]] ValueType GetValueType() const override;
    [[nodiscard]] bool AsBool() const override;
    [[nodiscard]] double AsDouble() const override;
    [[nodiscard]] std::string AsString() const override;
    [[nodiscard]] std::vector<std::unique_ptr<Value>> AsArray() const override;

private:
    bool value_;
};