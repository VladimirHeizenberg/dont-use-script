#pragma once

#include <string>
#include <memory>

#include "Statement.h"
#include "AssignType.h"
#include "src/ast/expression/Expression.h"
#include "src/ast/expression/BinExpr.h"
#include "src/ast/expression/LiteralExpr.h"
#include "src/executor/Context.h"


namespace itmo_script::ast {


class AssignStmt: public Statement {
public:
    AssignStmt(const std::string& name, 
                       std::unique_ptr<Expression> expr,
                       AssignType type)
    : name_(name) {
        if (type == AssignType::kAssign) {
            expr_ = std::move(expr);
        } else {
            expr_ = std::make_unique<BinExpr>(
                to_operators_table_.at(type), 
                std::make_unique<LiteralExpr>(name), 
                std::move(expr)
            );
        }
    }

    StatementResultProxy Execute(executor::Context& context) override {
        auto result = expr_->Evaluate(context);
        context.table().Set(name_, result);
        return normal_result();
    }
private:
    std::string name_;
    std::unique_ptr<Expression> expr_;
    const inline static std::unordered_map<AssignType, OperationType> to_operators_table_ = {
        {AssignType::kPlusAssign,  OperationType::kPlusOp},
        {AssignType::kMinusAssign, OperationType::kMinusOp},
        {AssignType::kMulAssign,   OperationType::kMulOp},
        {AssignType::kDivAssign,   OperationType::kDivOp},
    };
};

} // namespace itmo_script::ast