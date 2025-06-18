#pragma once

#include <memory>

#include "src/AST/VariablesTable.h"
#include "src/executor/Context.h"
#include "src/executor/StatementSource.h"


class Executor {
public:
    Executor(std::unique_ptr<StatementSource>&& source,
             std::istream& input_stream,
             std::ostream& output_stream);
    bool Execute();
private:
    void AddMathFunctions();
    void AddStringFunctions();
    void AddArrayFunctions();
    void AddIOFunctions();
    std::unique_ptr<StatementSource> source_;
    VariablesTable table_;
    Context context_;
};
