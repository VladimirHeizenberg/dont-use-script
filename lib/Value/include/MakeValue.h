#pragma once

#include "ValueInterface.h"

ValuePtr MakeStringValue(const std::string& value);
ValuePtr MakeDoubleValue(double num);
ValuePtr MakeBoolValue(bool value);