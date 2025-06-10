#include "../include/Operations.h"

#include <stdexcept>
#include <cmath>
#include <functional>
#include <map>

#include "../include/ValueInterface.h"
#include "../include/MakeValue.h"

using OperandsType = std::pair<ValueType, ValueType>;
using Function = std::function<ValuePtr(const ValuePtr&, const ValuePtr&)>;

ValuePtr AddDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDoubleValue(left->AsDouble() + right->AsDouble());
}

ValuePtr SubtractDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDoubleValue(left->AsDouble() - right->AsDouble());
}

ValuePtr MultiplyDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDoubleValue(left->AsDouble() * right->AsDouble());
}

ValuePtr DivideDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDoubleValue(left->AsDouble() / right->AsDouble());
}

ValuePtr RemainderDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDoubleValue(
        static_cast<double>(
            static_cast<long long>(left->AsDouble()) % static_cast<long long>(right->AsDouble())
        )
    );
}

ValuePtr PowDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeDoubleValue(std::pow(left->AsDouble(), right->AsDouble()));
}

ValuePtr EqualsDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeBoolValue(left->AsDouble() == right->AsDouble());
}

ValuePtr LessDoubles(const ValuePtr& left, const ValuePtr& right) {
    return MakeBoolValue(left->AsDouble() < right->AsDouble());
}

// Tables

static const std::map<OperandsType, Function> AddTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue},&AddDoubles},
    {{ValueType::kBoolValue, ValueType::kBoolValue},    &AddDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},  &AddDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},  &AddDoubles},
    {
        {ValueType::kStringValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) -> ValuePtr {
            return MakeStringValue(left->AsString() + right->AsString());
        }
    },
};

static const std::map<OperandsType, Function> SubtractTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &SubtractDoubles},
    {{ValueType::kBoolValue, ValueType::kBoolValue},     &SubtractDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &SubtractDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &SubtractDoubles},
    {
        {ValueType::kStringValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) -> ValuePtr {
            if (left->AsString().ends_with(right->AsString())) {
                return MakeStringValue(left->AsString() + right->AsString());
            }
        }
    },
};

static const std::map<OperandsType, Function> MultiplyTable = {
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
            return MakeStringValue(result);
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
            return MakeStringValue(result);
        }
    },
};

static const std::map<OperandsType, Function> DivideTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &DivideDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &DivideDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &DivideDoubles},
};

static const std::map<OperandsType, Function> RemainderTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &RemainderDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &RemainderDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &RemainderDoubles},
};

static const std::map<OperandsType, Function> PowTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &PowDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &PowDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &PowDoubles},
};

static const std::map<OperandsType, Function> EqualsTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &EqualsDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &EqualsDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &EqualsDoubles},
    {
        {ValueType::kStringValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) {
            return MakeBoolValue(left->AsString() == right->AsString());
        }
    }
};

static const std::map<OperandsType, Function> LessTable = {
    {{ValueType::kDoubleValue, ValueType::kDoubleValue}, &LessDoubles},
    {{ValueType::kBoolValue, ValueType::kDoubleValue},   &LessDoubles},
    {{ValueType::kDoubleValue, ValueType::kBoolValue},   &LessDoubles},
    {
        {ValueType::kStringValue, ValueType::kStringValue},
        [](const ValuePtr& left, const ValuePtr& right) {
            return MakeBoolValue(left->AsString() < right->AsString());
        }
    }
};

// Aliases

ValuePtr BinaryOperation(const ValuePtr& left, const ValuePtr& right,
                         const std::map<OperandsType, Function>& table) {
    ValueType left_type = left->GetValueType();
    ValueType right_type = right->GetValueType();
    if (!table.contains({left_type, right_type})) {
        throw std::runtime_error("Wrong operands");
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
    return BinaryOperation(left, right, EqualsTable);
}

ValuePtr NotEquals(const ValuePtr& left, const ValuePtr& right) {
    return MakeBoolValue(!Equals(left, right)->AsBool());
}

ValuePtr Less(const ValuePtr& left, const ValuePtr& right) {
    return BinaryOperation(left, right, LessTable);
}

ValuePtr LessOrEqual(const ValuePtr& left, const ValuePtr& right) {
    return MakeBoolValue(Less(left, right)->AsBool() | Equals(left, right)->AsBool());
}

ValuePtr Greater(const ValuePtr& left, const ValuePtr& right) {
    return MakeBoolValue(!LessOrEqual(left, right)->AsBool());
}

ValuePtr GreaterOrEqual(const ValuePtr& left, const ValuePtr& right) {
    return MakeBoolValue(!Less(left, right)->AsBool());
}

ValuePtr LogicalAnd(const ValuePtr& left, const ValuePtr& right) {
    return MakeBoolValue(left->AsBool() & right ->AsBool());
}

ValuePtr LogicalOr(const ValuePtr& left, const ValuePtr& right) {
    return MakeBoolValue(left->AsBool() | right->AsBool());
}

ValuePtr LogicalNot(const ValuePtr& left) {
    return MakeBoolValue(!left->AsBool());
}