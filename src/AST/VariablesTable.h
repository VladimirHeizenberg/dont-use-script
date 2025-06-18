#pragma once

#include <unordered_map>
#include <string>

#include "src/value/headers/ValueDeclaration.h"

class VariablesTable {
public:
    VariablesTable(VariablesTable& parent)
        : parent_(&parent) {}

    VariablesTable() = default;

    void Set(const std::string& name, ValuePtr value) {
        variables[name] = std::move(value);
    }

    const ValuePtr& Get(const std::string& name) const {
        bool this_contains = variables.contains(name);
        if (!this_contains && parent_ != nullptr) return parent_->Get(name);
        return variables.at(name);
    }

    bool Contains(const std::string& name) const {
        bool this_contains = variables.contains(name);
        if (!this_contains && parent_ != nullptr) return parent_->Contains(name);
        return this_contains;
    }
private:
    std::unordered_map<std::string, ValuePtr> variables;
    VariablesTable* parent_ = nullptr;
};