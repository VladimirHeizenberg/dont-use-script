#pragma once

#include <random>
#include <cmath>
#include <sstream>

#include "VariablesTable.h"
#include "src/value/MakeValue.h"
#include "src/value/Operation.h"


namespace itmo_script::executor {

class Initializer {
public:
    Initializer(VariablesTable& table, Context& context)
    : table_(table) {}

    void AddLenFunction() {
        table_.Set(
            "len",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1) {
                    throw std::runtime_error("Wrong arguments for len");
                }
                if (args[0]->GetValueType() == value::ValueType::kStringValue) {
                    return value::MakeDouble(args[0]->AsString().size());
                }
                if (args[0]->GetValueType() == value::ValueType::kArrayValue) {
                    return value::MakeDouble(args[0]->AsArray().size());
                }
                throw std::runtime_error("Wrong arguments for len");
            })
        );
    }

    void AddMathFunctions() {
        table_.Set(
            "abs",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kDoubleValue) {
                throw std::runtime_error("Wrong arguments for abs");
                }
                return value::MakeDouble(std::abs(args[0]->AsDouble()));
            })
        );
        table_.Set(
            "ceil",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kDoubleValue) {
                throw std::runtime_error("Wrong arguments for len");
                }
                return value::MakeDouble(std::ceil(args[0]->AsDouble()));
            })
        );
        table_.Set(
            "floor",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kDoubleValue) {
                throw std::runtime_error("Wrong arguments for floor");
                }
                return value::MakeDouble(std::floor(args[0]->AsDouble()));
            })
        );
        table_.Set(
            "round",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kDoubleValue) {
                throw std::runtime_error("Wrong arguments for round");
                }
                return value::MakeDouble(std::round(args[0]->AsDouble()));
            })
        );
        table_.Set(
            "sqrt",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kDoubleValue) {
                throw std::runtime_error("Wrong arguments for sqrt");
                }
                return value::MakeDouble(std::sqrt(args[0]->AsDouble()));
            })
        );
        table_.Set(
            "rnd",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kDoubleValue) {
                throw std::runtime_error("Wrong arguments for rnd");
                }
                return value::MakeDouble(rand() % (static_cast<long long>(args[0]->AsDouble())));
            })
        );
        table_.Set(
            "parse_num",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kStringValue) {
                throw std::runtime_error("Wrong arguments for parse_num");
                }
                return value::MakeDouble(std::stod(args[0]->AsString()));
            })
        );
        table_.Set(
            "to_string",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kDoubleValue) {
                throw std::runtime_error("Wrong arguments for to_string");
                }
                return value::MakeString(args[0]->AsString());
            })
        );
    }

    void AddStringFunctions() {
        table_.Set(
            "lower",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kStringValue) {
                    throw std::runtime_error("Wrong arguments for lower");
                }
                std::string s = args[0]->AsString();
                std::transform(
                    s.begin(), s.end(), s.begin(),
                    [](unsigned char c){ return std::tolower(c); }
                );
                return value::MakeString(s);
            })
        );
        table_.Set(
            "upper",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kStringValue) {
                    throw std::runtime_error("Wrong arguments for upper");
                }
                std::string s = args[0]->AsString();
                std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c); });
                return value::MakeString(s);
            })
        );
        table_.Set(
            "split",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 2 || args[0]->GetValueType() != value::ValueType::kStringValue ||
                    args[1]->GetValueType() != value::ValueType::kStringValue) {
                    throw std::runtime_error("Wrong arguments for split");
                }
                std::string s = args[0]->AsString();
                std::string delim = args[1]->AsString();
                std::vector<value::ValuePtr> result;
                size_t pos = 0;
                while (true) {
                    size_t next = s.find(delim, pos);
                    if (next == std::string::npos) {
                        result.push_back(value::MakeString(s.substr(pos)));
                        break;
                    }
                    result.push_back(value::MakeString(s.substr(pos, next - pos)));
                    pos = next + delim.size();
                }
                auto array = value::MakeArray();
                array->AsArray() = std::move(result);
                return array;
            })
        );
        table_.Set(
            "join",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 2 || args[0]->GetValueType() != value::ValueType::kArrayValue ||
                    args[1]->GetValueType() != value::ValueType::kStringValue) {
                    throw std::runtime_error("Wrong arguments for join");
                }
                std::string delim = args[1]->AsString();
                auto& arr = args[0]->AsArray();
                std::stringstream ss;
                for (size_t i = 0; i < arr.size(); ++i) {
                    ss << arr[i]->AsString();
                    if (i + 1 < arr.size())
                        ss << delim;
                }
                return value::MakeString(ss.str());
            })
        );
        table_.Set(
            "replace",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 3 || args[0]->GetValueType() != value::ValueType::kStringValue ||
                    args[1]->GetValueType() != value::ValueType::kStringValue ||
                    args[2]->GetValueType() != value::ValueType::kStringValue) {
                    throw std::runtime_error("Wrong arguments for replace");
                }
                std::string s = args[0]->AsString();
                std::string& from = args[1]->AsString();
                std::string& to = args[2]->AsString();
                size_t pos = 0;
                while ((pos = s.find(from, pos)) != std::string::npos) {
                    s.replace(pos, from.length(), to);
                    pos += to.length();
                }
                return value::MakeString(s);
            })
        );
    }

    void AddArrayFunctions() {
        table_.Set(
            "range",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 3 || args[0]->GetValueType() != value::ValueType::kDoubleValue ||
                    args[1]->GetValueType() != value::ValueType::kDoubleValue ||
                    args[2]->GetValueType() != value::ValueType::kDoubleValue) {
                    throw std::runtime_error("Wrong arguments for range");
                }
                auto array = value::MakeArray();
                auto init = value::MakeDouble(args[0]->AsDouble());
                while (value::Less(init, args[1])->AsDouble()) {
                    array->AsArray().push_back(init);
                    init = value::Add(init, args[2]);
                }
                return array;
            })
        );

        table_.Set(
            "push",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 2 || args[0]->GetValueType() != value::ValueType::kArrayValue) {
                    throw std::runtime_error("Wrong arguments for push");
                }
                args[0]->AsArray().push_back(args[1]);
                return value::MakeNull();
            })
        );

        table_.Set(
            "pop",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kArrayValue) {
                    throw std::runtime_error("Wrong arguments for pop");
                }
                if (args[0]->AsArray().empty()) {
                    throw std::runtime_error("Pop() called for empty array");
                }
                args[0]->AsArray().pop_back();
                return value::MakeNull();
            })
        );

        table_.Set(
            "insert",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 3 || args[0]->GetValueType() != value::ValueType::kArrayValue ||
                    args[1]->GetValueType() != value::ValueType::kDoubleValue) {
                    throw std::runtime_error("Wrong arguments for insert");
                }
                auto& array = args[0]->AsArray();
                if (args[1]->AsDouble() >= array.size()) {
                    throw std::runtime_error("list index out of range");
                }
                array.insert(array.begin() + args[1]->AsDouble(), args[2]);
                return value::MakeNull();
            })
        );

        table_.Set(
            "remove",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 2 || args[0]->GetValueType() != value::ValueType::kArrayValue ||
                    args[1]->GetValueType() != value::ValueType::kDoubleValue) {
                    throw std::runtime_error("Wrong arguments for remove");
                }
                auto& array = args[0]->AsArray();
                if (args[1]->AsDouble() >= array.size()) {
                    throw std::runtime_error("list index out of range");
                }
                array.erase(array.begin() + args[1]->AsDouble());
                return value::MakeNull();
            })
        );

        table_.Set(
            "sort",
            value::MakeSystemFunction([](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1 || args[0]->GetValueType() != value::ValueType::kArrayValue) {
                    throw std::runtime_error("Wrong arguments for sort");
                }
                auto comparator = [](const value::ValuePtr& left, const value::ValuePtr& right) {
                    return value::Less(left, right)->AsDouble();
                };
                auto& array = args[0]->AsArray();
                std::sort(array.begin(), array.end(), comparator);
                return value::MakeNull();
            })
        );
    }
private:
    VariablesTable& table_;
};

} // namespace itmo_script::executor