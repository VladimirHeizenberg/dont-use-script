#pragma once

#include "Statement.h"
#include "ast/expression/Expression.h"

#include "value/MakeValue.h"


namespace itmo_script::ast {

class ForStmt: public Statement {
public:
    ForStmt(std::string variable_name,
                 std::unique_ptr<Expression> for_expression,
                 std::unique_ptr<Statement> for_statement)
    : variable_name_(std::move(variable_name))
    , for_expression_(std::move(for_expression))
    , for_statement_(std::move(for_statement)) {}

    StatementResultProxy Execute(executor::Context &context) override {
        auto expression = for_expression_->Evaluate(context);
        if (expression->GetValueType() == value::ValueType::kArrayValue) {
            for (auto& iter_value : expression->AsArray()) {
                context.table().Set(variable_name_, iter_value);
                StatementResultProxy tmp_result = for_statement_->Execute(context);
                if (tmp_result.result == StatementResult::kBreak) {
                    break;
                }
                if (tmp_result.result == StatementResult::kReturn) {
                    return tmp_result;
                }
            }
            return {nullptr, StatementResult::kNormal};
        }
        if (expression->GetValueType() == value::ValueType::kStringValue) {
            for (auto& iter_value : expression->AsString()) {
                context.table().Set(variable_name_, value::MakeString(std::string{iter_value}));
                StatementResultProxy tmp_result = for_statement_->Execute(context);
                if (tmp_result.result == StatementResult::kBreak) {
                    break;
                }
                if (tmp_result.result == StatementResult::kReturn) {
                    return tmp_result;
                }
            }
            return {nullptr, StatementResult::kNormal};
        }
        throw std::runtime_error("Invalid expression for cycle for");
    }

private:
    std::string variable_name_;
    std::unique_ptr<Expression> for_expression_;
    std::unique_ptr<Statement> for_statement_;
};

}