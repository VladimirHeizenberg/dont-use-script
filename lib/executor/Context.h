#pragma once

#include <iostream>

#include "AST/VariablesTable.h"


class Context {
public:
    Context(std::istream& input,
            std::ostream& output,
            VariablesTable& table)
    : input_(input)
    , output_(output)
    , table_(table) {}

    std::istream& input() {return input_;}
    std::ostream& output() {return output_;}
    VariablesTable& table() {return table_;}
    
private:
    std::istream& input_;
    std::ostream& output_;
    VariablesTable& table_;
};