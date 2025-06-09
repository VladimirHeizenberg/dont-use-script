#pragma once

#include "ValueInterface.h"
#include <string>
#include <vector>
#include <memory>

class StringValue final : public Value {
public:
    explicit StringValue(const std::string& value);

    [[nodiscard]] ValueType GetValueType() const override;
    [[nodiscard]] bool AsBool() const override;
    [[nodiscard]] std::string AsString() const override;
    [[nodiscard]] std::vector<std::unique_ptr<Value>> AsArray() const override;
    [[nodiscard]] double AsDouble() const override;

private:
    std::string value_;
};