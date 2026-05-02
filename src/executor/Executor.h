#pragma once

#include <memory>

#include "VariablesTable.h"
#include "Initializer.h"
#include "executor/Context.h"
#include "executor/StatementSource.h"

namespace dont_use_script::executor {

class Executor {
public:
    Executor(std::unique_ptr<StatementSource>&& source,
                   std::istream& input_stream,
                   std::ostream& output_stream)
    : source_(std::move(source))
    , table_()
    , context_(Context(input_stream, output_stream, table_)) {
        Initializer initializer(table_, context_);
        initializer.AddArrayFunctions();
        initializer.AddMathFunctions();
        initializer.AddStringFunctions();
        initializer.AddLenFunction();
        AddIOFunctions();
    }

    bool Execute() {
        while (!source_->eof()) {
            source_->Get()->Execute(context_);
        }
        return true;
    }
private:

    void AddIOFunctions() {
        table_.Set(
            "print",
            value::MakeSystemFunction([this](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1) throw std::runtime_error("Wrong arguments for print");
                context_.output() << args[0]->AsString();
                return value::MakeNull();
            })
        );

        table_.Set(
            "println",
            value::MakeSystemFunction([this](const std::vector<value::ValuePtr>& args) {
                if (args.size() != 1) throw std::runtime_error("Wrong arguments for println");
                context_.output() << args[0]->AsString() << std::endl;
                return value::MakeNull();
            })
        );

        table_.Set(
            "read",
            value::MakeSystemFunction([this](const std::vector<value::ValuePtr>& args) {
                if (!args.empty()) throw std::runtime_error("Wrong arguments for read");
                std::string line;
                std::getline(context_.input(), line);
                return value::MakeString(line);
            })
        );
    }

    std::unique_ptr<StatementSource> source_;
    VariablesTable table_;
    Context context_;
};

} // namespace dont_use_script::executor