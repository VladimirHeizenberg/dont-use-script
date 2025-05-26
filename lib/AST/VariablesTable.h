#pragma once

#include <unordered_map>
#include <string>
#include <any>

class VariablesTable {
public:
    void Set(const std::string& name, double value) {
        variables[name] = value;
    }

    double Get(const std::string& name) {
        return variables.at(name);
    }

    bool Contains(const std::string& name) {
        return variables.contains(name);
    }
private:
    std::unordered_map<std::string, double> variables;
};