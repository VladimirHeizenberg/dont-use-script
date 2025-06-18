#include "include/executor/Executor.h"

#include <random>
#include <cmath>
#include <sstream>

#include "src/value/headers/MakeValue.h"
#include "src/value/headers/Operations.h"

Executor::Executor(std::unique_ptr<StatementSource>&& source,
                   std::istream& input_stream,
                   std::ostream& output_stream)
    : source_(std::move(source))
    , table_()
    , context_(Context(input_stream, output_stream, table_)) {
    // system functions:
    AddMathFunctions();
    AddStringFunctions();
    AddArrayFunctions();
    AddIOFunctions();
    // len
    table_.Set(
        "len",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1) {
                throw std::runtime_error("Wrong arguments for len");
            }
            if (args[0]->GetValueType() == ValueType::kStringValue) {
                return MakeDoubleValue(args[0]->AsString().size());
            }
            if (args[0]->GetValueType() == ValueType::kArrayValue) {
                return MakeDoubleValue(args[0]->AsArray().size());
            }
            throw std::runtime_error("Wrong arguments for len");
        }));
}

bool Executor::Execute() {
    while (!source_->eof()) {
        source_->Get()->execute(context_);
    }
    return true;
}

void Executor::AddMathFunctions() {
    table_.Set(
        "abs",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kDoubleValue) {
               throw std::runtime_error("Wrong arguments for abs");
            }
            return MakeDoubleValue(std::abs(args[0]->AsDouble()));
        })
    );
    table_.Set(
        "ceil",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kDoubleValue) {
               throw std::runtime_error("Wrong arguments for len");
            }
            return MakeDoubleValue(std::ceil(args[0]->AsDouble()));
        })
    );
    table_.Set(
        "floor",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kDoubleValue) {
               throw std::runtime_error("Wrong arguments for floor");
            }
            return MakeDoubleValue(std::floor(args[0]->AsDouble()));
        })
    );
    table_.Set(
        "round",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kDoubleValue) {
               throw std::runtime_error("Wrong arguments for round");
            }
            return MakeDoubleValue(std::round(args[0]->AsDouble()));
        })
    );
    table_.Set(
        "sqrt",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kDoubleValue) {
               throw std::runtime_error("Wrong arguments for sqrt");
            }
            return MakeDoubleValue(std::sqrt(args[0]->AsDouble()));
        })
    );
    table_.Set(
        "rnd",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kDoubleValue) {
               throw std::runtime_error("Wrong arguments for rnd");
            }
            return MakeDoubleValue(rand() % (static_cast<long long>(args[0]->AsDouble())));
        })
    );
    table_.Set(
        "parse_num",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kStringValue) {
               throw std::runtime_error("Wrong arguments for parse_num");
            }
            return MakeDoubleValue(std::stod(args[0]->AsString()));
        })
    );
    table_.Set(
        "to_string",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kDoubleValue) {
               throw std::runtime_error("Wrong arguments for to_string");
            }
            return MakeStringValue(args[0]->AsString());
        })
    );
}

void Executor::AddStringFunctions() {
    table_.Set(
        "lower",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kStringValue) {
                throw std::runtime_error("Wrong arguments for lower");
            }
            std::string s = args[0]->AsString();
            std::transform(
                s.begin(), s.end(), s.begin(),
                [](unsigned char c){ return std::tolower(c); }
            );
            return MakeStringValue(s);
        })
    );
    table_.Set(
        "upper",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kStringValue) {
                throw std::runtime_error("Wrong arguments for upper");
            }
            std::string s = args[0]->AsString();
            std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c); });
            return MakeStringValue(s);
        })
    );
    table_.Set(
        "split",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 2 || args[0]->GetValueType() != ValueType::kStringValue ||
                args[1]->GetValueType() != ValueType::kStringValue) {
                throw std::runtime_error("Wrong arguments for split");
            }
            std::string s = args[0]->AsString();
            std::string delim = args[1]->AsString();
            std::vector<ValuePtr> result;
            size_t pos = 0;
            while (true) {
                size_t next = s.find(delim, pos);
                if (next == std::string::npos) {
                    result.push_back(MakeStringValue(s.substr(pos)));
                    break;
                }
                result.push_back(MakeStringValue(s.substr(pos, next - pos)));
                pos = next + delim.size();
            }
            auto array = MakeArrayValue();
            array->AsArray() = std::move(result);
            return array;
        })
    );
    table_.Set(
        "join",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 2 || args[0]->GetValueType() != ValueType::kArrayValue ||
                args[1]->GetValueType() != ValueType::kStringValue) {
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
            return MakeStringValue(ss.str());
        })
    );
    table_.Set(
        "replace",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 3 || args[0]->GetValueType() != ValueType::kStringValue ||
                args[1]->GetValueType() != ValueType::kStringValue ||
                args[2]->GetValueType() != ValueType::kStringValue) {
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
            return MakeStringValue(s);
        })
    );
}

void Executor::AddArrayFunctions() {
    table_.Set(
        "range",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 3 || args[0]->GetValueType() != ValueType::kDoubleValue ||
                args[1]->GetValueType() != ValueType::kDoubleValue||
                args[2]->GetValueType() != ValueType::kDoubleValue) {
                throw std::runtime_error("Wrong arguments for push");
            }
            auto array = MakeArrayValue();
            auto init = MakeDoubleValue(args[0]->AsDouble());
            while (Less(init, args[1])->AsDouble()) {
                array->AsArray().push_back(init);
                init = Add(init, args[2]);
            }
            return array;
        })
    );
    table_.Set(
        "push",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 2 || args[0]->GetValueType() != ValueType::kArrayValue) {
               throw std::runtime_error("Wrong arguments for push");
            }
            args[0]->AsArray().push_back(args[1]);
            return MakeNullValue();
        })
    );
    table_.Set(
        "pop",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kArrayValue) {
               throw std::runtime_error("Wrong arguments for pop");
            }
            if (args[0]->AsArray().size() < 1) {
                throw std::runtime_error("Pop() called for empty array");
            }
            args[0]->AsArray().pop_back();
            return MakeNullValue();
        })
    );
    table_.Set(
        "insert",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 3 || args[0]->GetValueType() != ValueType::kArrayValue ||
                args[1]->GetValueType() != ValueType::kDoubleValue) {
               throw std::runtime_error("Wrong arguments for pop");
            }
            auto& array = args[0]->AsArray();
            if (args[1]->AsDouble() >= array.size()) {
                throw std::runtime_error("list index out of range");
            }
            array.insert(array.begin() + args[1]->AsDouble(), args[2]);
            return MakeNullValue();
        })
    );
    table_.Set(
        "remove",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 2 || args[0]->GetValueType() != ValueType::kArrayValue ||
                args[1]->GetValueType() != ValueType::kDoubleValue) {
               throw std::runtime_error("Wrong arguments for pop");
            }
            auto& array = args[0]->AsArray();
            if (args[1]->AsDouble() >= array.size()) {
                throw std::runtime_error("list index out of range");
            }
            array.erase(array.begin() + args[1]->AsDouble());
            return MakeNullValue();
        })
    );
    table_.Set(
        "sort",
        MakeSystemFunctionValue([](const std::vector<ValuePtr>& args) {
            if (args.size() != 1 || args[0]->GetValueType() != ValueType::kArrayValue) {
               throw std::runtime_error("Wrong arguments for pop");
            }
            auto comparator = [](const ValuePtr& left, const ValuePtr& right) {
                return Less(left, right)->AsDouble();
            };
            auto& array = args[0]->AsArray();
            sort(array.begin(), array.end(), comparator);
            return MakeNullValue();
        })
    );
}

void Executor::AddIOFunctions() {
    table_.Set(
        "print",
        MakeSystemFunctionValue([this](const std::vector<ValuePtr>& args) {
            if (args.size() != 1) throw std::runtime_error("Wrong arguments for print");
            context_.output() << args[0]->AsString();
            return MakeNullValue();
        })
    );
    table_.Set(
        "println",
        MakeSystemFunctionValue([this](const std::vector<ValuePtr>& args) {
            if (args.size() != 1) throw std::runtime_error("Wrong arguments for println");
            context_.output() << args[0]->AsString() << std::endl;
            return MakeNullValue();
        })
    );
    table_.Set(
        "read",
        MakeSystemFunctionValue([this](const std::vector<ValuePtr>& args) {
            if (!args.empty()) throw std::runtime_error("Wrong arguments for read");
            std::string line;
            std::getline(context_.input(), line);
            return MakeStringValue(line);
        })
    );
}