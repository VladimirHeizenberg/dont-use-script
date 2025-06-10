#pragma once

#include <memory>
#include "ValueInterface.h"

// Arithmetic operations
ValuePtr Add(const ValuePtr& left, const ValuePtr& right);
ValuePtr Subtract(const ValuePtr& left, const ValuePtr& right);
ValuePtr Multiply(const ValuePtr& left, const ValuePtr& right);
ValuePtr Divide(const ValuePtr& left, const ValuePtr& right);
ValuePtr Remainder(const ValuePtr& left, const ValuePtr& right);
ValuePtr Power(const ValuePtr& left, const ValuePtr& right);

// Logical operations
ValuePtr Equals(const ValuePtr& left, const ValuePtr& right);
ValuePtr NotEquals(const ValuePtr& left, const ValuePtr& right);
ValuePtr Less(const ValuePtr& left, const ValuePtr& right);
ValuePtr LessOrEqual(const ValuePtr& left, const ValuePtr& right);
ValuePtr Greater(const ValuePtr& left, const ValuePtr& right);
ValuePtr GreaterOrEqual(const ValuePtr& left, const ValuePtr& right);
ValuePtr LogicalAnd(const ValuePtr& left, const ValuePtr& right);
ValuePtr LogicalOr(const ValuePtr& left, const ValuePtr& right);
ValuePtr LogicalNot(const ValuePtr& left);
