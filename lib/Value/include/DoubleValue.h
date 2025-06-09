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
    [[nodiscard]] std::string AsString() const override;
    [[nodiscard]] std::vector<std::unique_ptr<Value>> AsArray() const override;

private:
    double num_;
};
