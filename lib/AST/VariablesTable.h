#pragma once

#include <unordered_map>
#include <string>

#include "../Value/include/ValueDeclaration.h"

class VariablesTable {
public:
    void Set(const std::string& name, ValuePtr value) {
        variables[name] = std::move(value);
    }

    const ValuePtr& Get(const std::string& name) const {
        return variables.at(name);
    }

    bool Contains(const std::string& name) const {
        return variables.contains(name);
    }
private:
    std::unordered_map<std::string, ValuePtr> variables;

};