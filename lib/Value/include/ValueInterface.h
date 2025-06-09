#pragma once

#include <string>
#include <vector>
#include <memory>

enum class ValueType {
    kArrayValue,
    kBoolValue,
    kDoubleValue,
    kStringValue,
};

class Value {
public:
    [[nodiscard]] virtual ValueType GetValueType() const = 0;
    [[nodiscard]] virtual bool AsBool() const = 0;
    [[nodiscard]] virtual double AsDouble() const = 0;
    [[nodiscard]] virtual std::string AsString() const = 0;
    [[nodiscard]] virtual std::vector<std::unique_ptr<Value>> AsArray() const = 0;

    virtual ~Value() = default;
};

using ValuePtr = std::shared_ptr<Value>;