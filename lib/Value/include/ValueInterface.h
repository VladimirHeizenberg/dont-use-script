#pragma once

#include <string>
#include <vector>
#include <memory>

#include "ValueDeclaration.h"
#include "../../executor/Context.h"


enum class ValueType {
    kArrayValue,
    kBoolValue,
    kDoubleValue,
    kFunctionValue,
    kStringValue,
    kNullValue,
};

inline std::string type_to_string(ValueType type) {
    switch (type) {
        case ValueType::kArrayValue: return "Array";
        case ValueType::kBoolValue: return "Bool";
        case ValueType::kDoubleValue: return "Double";
        case ValueType::kFunctionValue: return "Function";
        case ValueType::kStringValue: return "String";
    }
    return "";
}

class Value {
public:
    [[nodiscard]] virtual ValueType GetValueType() = 0;
    [[nodiscard]] virtual bool AsBool() = 0;
    [[nodiscard]] virtual double AsDouble() = 0;
    [[nodiscard]] virtual std::string& AsString() = 0;
    [[nodiscard]] virtual std::vector<ValuePtr>& AsArray() = 0;
    [[nodiscard]] virtual ValuePtr AsFunctionCall(const std::vector<ValuePtr>& args, Context& context) = 0;

    virtual ~Value() = default;
};
