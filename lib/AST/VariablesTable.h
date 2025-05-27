#pragma once

#include <unordered_map>
#include <string>
#include <any>

#include "Value.h"

class VariablesTable {
public:
    void Set(const std::string& name, const Value& value) {
        variables[name] = value;
    }

    const Value& Get(const std::string& name) {
        return variables.at(name);
    }

    bool Contains(const std::string& name) {
        return variables.contains(name);
    }
private:
    std::unordered_map<std::string, Value> variables;
};