#pragma once

#include <memory>

#include "src/executor/Context.h"
#include "src/executor/StatementSource.h"


class Executor {
public:
    Executor(std::unique_ptr<StatementSource>&& source,
             std::istream& input_stream,
             std::ostream& output_stream)
    : source_(std::move(source))
    , table_(VariablesTable())
    , context_(Context(input_stream, output_stream, table_)) {}

    bool Execute() {
        while (!source_->eof()) {
            source_->Get()->execute(context_);
        }
        return true;
    }
private:
    std::unique_ptr<StatementSource> source_;
    VariablesTable table_;
    Context context_;
};
