#include "Operation.h"

#include <stdexcept>
#include <cmath>
#include <functional>
#include <map>
#include <format>

#include "Value.h"
#include "MakeValue.h"


namespace itmo_script::value {

using OperandsType = std::pair<ValueType, ValueType>;
using FunctionT = std::function<ValuePtr(const ValuePtr&, const ValuePtr&)>;

ValuePtr AddDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDouble(left->AsDouble() + right->AsDouble());
}

ValuePtr SubtractDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDouble(left->AsDouble() - right->AsDouble());
}

ValuePtr MultiplyDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDouble(left->AsDouble() * right->AsDouble());
}

ValuePtr DivideDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDouble(left->AsDouble() / right->AsDouble());
}

ValuePtr RemainderDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDouble(
        static_cast<double>(
            static_cast<long long>(left->AsDouble()) % static_cast<long long>(right->AsDouble())
        )
    );
}

ValuePtr PowDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDouble(std::pow(left->AsDouble(), right->AsDouble()));
}

ValuePtr EqualsDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeBool(left->AsDouble() == right->AsDouble());
}

ValuePtr LessDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeBool(left->AsDouble() < right->AsDouble());
}

// Tables

static const std::map<OperandsType, FunctionT> AddTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue},&AddDoubles},
    {{ValueType::kBoolValue, ValueType::kBoolValue},    &AddDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},  &AddDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},  &AddDoubles},
    {
        {ValueType::kStringValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) -> ValuePtr {
            return MakeString(left->AsString() + right->AsString());
        }
    },
    {
        {ValueType::kArrayValue, ValueType::kArrayValue},
        [](const ValuePtr& left, const ValuePtr& right) -> ValuePtr {
            std::vector<ValuePtr> values = left->AsArray();
            values.insert(values.end(), right->AsArray().begin(), right->AsArray().end());
            return MakeArray(std::move(values));
        }
    },
};

static const std::map<OperandsType, FunctionT> SubtractTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &SubtractDoubles},
    {{ValueType::kBoolValue, ValueType::kBoolValue},     &SubtractDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &SubtractDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &SubtractDoubles},
    {
        {ValueType::kStringValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) -> ValuePtr {
            if (left->AsString().ends_with(right->AsString())) {
                auto& left_str = left->AsString();
                return MakeString(std::string(left_str.begin(), left_str.begin() + right->AsString().size()));
            }
            return MakeString(left->AsString());
        }
    },
};

static const std::map<OperandsType, FunctionT> MultiplyTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue},&MultiplyDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},  &MultiplyDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},  &MultiplyDoubles},
    {{ValueType::kBoolValue, ValueType::kBoolValue},    &MultiplyDoubles},
    {
        {ValueType::kStringValue, ValueType::kDoubleValue},
        [](const ValuePtr& left, const ValuePtr& right) -> ValuePtr {
            std::string result;
            result.reserve(left->AsString().size() * static_cast<size_t>(right->AsDouble()));
            for (int i = 0; i < right->AsDouble(); ++i) {
                result += left->AsString();
            }
            return MakeString(result);
        }
    },
    {
        {ValueType::kDoubleValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) -> ValuePtr {
            std::string result;
            result.reserve(right->AsString().size() * static_cast<size_t>(left->AsDouble()));
            for (int i = 0; i < left->AsDouble(); ++i) {
                result += right->AsString();
            }
            return MakeString(result);
        }
    },
    {
        {ValueType::kStringValue, ValueType::kBoolValue},
        [](const ValuePtr& left, const ValuePtr& right) -> ValuePtr {
            std::string result;
            result.reserve(left->AsString().size() * static_cast<size_t>(right->AsDouble()));
            for (int i = 0; i < right->AsDouble(); ++i) {
                result += left->AsString();
            }
            return MakeString(result);
        }
    },
    {
        {ValueType::kBoolValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) -> ValuePtr {
            std::string result;
            result.reserve(right->AsString().size() * static_cast<size_t>(left->AsDouble()));
            for (int i = 0; i < left->AsDouble(); ++i) {
                result += right->AsString();
            }
            return MakeString(result);
        }
    },
};

static const std::map<OperandsType, FunctionT> DivideTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &DivideDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &DivideDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &DivideDoubles},
};

static const std::map<OperandsType, FunctionT> RemainderTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &RemainderDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &RemainderDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &RemainderDoubles},
};

static const std::map<OperandsType, FunctionT> PowTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &PowDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &PowDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &PowDoubles},
};

static const std::map<OperandsType, FunctionT> EqualsTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &EqualsDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &EqualsDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &EqualsDoubles},
    {
        {ValueType::kStringValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) {
            return MakeBool(left->AsString() == right->AsString());
        }
    },
    {
        {ValueType::kArrayValue, ValueType::kArrayValue},
        [](const ValuePtr& left, const ValuePtr& right) {
            if (left->AsArray().size() != right->AsArray().size()) {
                return MakeBool(false);
            }
            size_t size_of_arrays = left->AsArray().size();
            auto array1 = left->AsArray();
            auto array2 = right->AsArray();
            for (int i = 0; i < size_of_arrays; ++i) {
                if (NotEquals(array1[i], array2[i])->AsBool()) {
                    return MakeBool(false);
                }
            }
            return MakeBool(true);
        }
    },
};

static const std::map<OperandsType, FunctionT> LessTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &LessDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &LessDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &LessDoubles},
    {
        {ValueType::kStringValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) {
            return MakeBool(left->AsString() < right->AsString());
        }
    }
};

// Aliases

ValuePtr BinaryOperation(const ValuePtr& left, const ValuePtr& right,
                         const std::map<OperandsType, FunctionT>& table) {
    ValueType left_type = left->GetValueType();
    ValueType right_type = right->GetValueType();
    if (!table.contains({left_type, right_type})) {
        throw std::runtime_error(
            std::format(
                "Wrong operands: {} and {}",
                type_to_string(left_type),
                type_to_string(right_type)
            )
        );
    }
    return table.at({left_type, right_type})(left, right);
}

ValuePtr Add(const ValuePtr& left, const ValuePtr& right) {
    return BinaryOperation(left, right, AddTable);
}

ValuePtr Subtract(const ValuePtr& left, const ValuePtr& right) {
    return BinaryOperation(left, right, SubtractTable);
}

ValuePtr Multiply(const ValuePtr& left, const ValuePtr& right) {
    return BinaryOperation(left, right, MultiplyTable);
}

ValuePtr Divide(const ValuePtr& left, const ValuePtr& right) {
    return BinaryOperation(left, right, DivideTable);
}

ValuePtr Remainder(const ValuePtr& left, const ValuePtr& right) {
    return BinaryOperation(left, right, RemainderTable);
}

ValuePtr Power(const ValuePtr& left, const ValuePtr& right) {
    return BinaryOperation(left, right, PowTable);
}

ValuePtr Equals(const ValuePtr& left, const ValuePtr& right) {
    ValueType left_type = left->GetValueType();
    ValueType right_type = right->GetValueType();
    if (left_type == ValueType::kNullValue && right_type == ValueType::kNullValue) {
        return MakeBool(true);
    }
    if (left_type == ValueType::kNullValue || right_type == ValueType::kNullValue) {
        return MakeBool(false);
    }
    if (!EqualsTable.contains({left_type, right_type})) {
        return MakeBool(false);
    }
    return EqualsTable.at({left_type, right_type})(left, right);
}

ValuePtr NotEquals(const ValuePtr& left, const ValuePtr& right) {
    return MakeBool(!Equals(left, right)->AsBool());
}

ValuePtr Less(const ValuePtr& left, const ValuePtr& right) {
    return BinaryOperation(left, right, LessTable);
}

ValuePtr LessOrEqual(const ValuePtr& left, const ValuePtr& right) {
    return MakeBool(Less(left, right)->AsBool() | Equals(left, right)->AsBool());
}

ValuePtr Greater(const ValuePtr& left, const ValuePtr& right) {
    return MakeBool(!LessOrEqual(left, right)->AsBool());
}

ValuePtr GreaterOrEqual(const ValuePtr& left, const ValuePtr& right) {
    return MakeBool(!Less(left, right)->AsBool());
}

ValuePtr LogicalAnd(const ValuePtr& left, const ValuePtr& right) {
    return MakeBool(left->AsBool() & right ->AsBool());
}

ValuePtr LogicalOr(const ValuePtr& left, const ValuePtr& right) {
    return MakeBool(left->AsBool() | right->AsBool());
}

ValuePtr LogicalNot(const ValuePtr& left) {
    return MakeBool(!left->AsBool());
}

} // namespace itmo_script::value