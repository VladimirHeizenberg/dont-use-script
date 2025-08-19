#pragma once

#include "VariablesTable.h"


namespace itmo_script::executor {

class Context {
public:
    Context(std::istream& input,
            std::ostream& output,
            VariablesTable& table)
    : input_(input)
    , output_(output)
    , table_(table) {}

    [[nodiscard]] std::istream& input() const {return input_;}
    [[nodiscard]] std::ostream& output() const {return output_;}
    [[nodiscard]] VariablesTable& table() const {return table_;}
    
private:
    std::istream& input_;
    std::ostream& output_;
    VariablesTable& table_;
};

} // namespace itmo_script::executor
