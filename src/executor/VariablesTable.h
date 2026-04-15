#pragma once

#include <unordered_map>
#include <string>


namespace dont_use_script::value {
    class Value;
    using ValuePtr = std::shared_ptr<Value>;
}

namespace dont_use_script::executor {

class VariablesTable {
public:
    VariablesTable(VariablesTable& parent)
        : parent_(&parent) {}

    VariablesTable() = default;

    void Set(const std::string& name, value::ValuePtr value) {
        variables[name] = std::move(value);
    }

    const value::ValuePtr& Get(const std::string& name) const {
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
    std::unordered_map<std::string, value::ValuePtr> variables;
    VariablesTable* parent_ = nullptr;
};

}