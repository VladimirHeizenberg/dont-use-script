#pragma once

#include "Statement.h"
#include "Expression.h"


class ForStatement: public StatementAST {
public:
    ForStatement(std::string variable_name,
                 std::unique_ptr<ExpressionAST> for_expression,
                 std::unique_ptr<StatementAST> for_statement)
    : variable_name_(std::move(variable_name))
    , for_expression_(std::move(for_expression))
    , for_statement_(std::move(for_statement)) {}

    StatementResultProxy execute(Context &context) override {
        auto expression = for_expression_->evaluate(context);
        if (expression->GetValueType() == ValueType::kArrayValue) {
            for (auto& iter_value : expression->AsArray()) {
                context.table().Set(variable_name_, iter_value);
                StatementResultProxy tmp_result = for_statement_->execute(context);
                if (tmp_result.result == StatementResult::kBreak) {
                    break;
                }
                if (tmp_result.result == StatementResult::kReturn) {
                    return tmp_result;
                }
            }
            return {nullptr, StatementResult::kNormal};
        }
        if (expression->GetValueType() == ValueType::kStringValue) {
            for (auto& iter_value : expression->AsString()) {
                context.table().Set(variable_name_, MakeStringValue(std::string{iter_value}));
                StatementResultProxy tmp_result = for_statement_->execute(context);
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
    std::unique_ptr<ExpressionAST> for_expression_;
    std::unique_ptr<StatementAST> for_statement_;
};