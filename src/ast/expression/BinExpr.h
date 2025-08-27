#pragma once

#include "Expression.h"
#include "OperationType.h"
#include "value/Operation.h"

namespace itmo_script::ast {

class BinExpr: public Expression {
public:
    BinExpr(OperationType operation, 
            std::unique_ptr<Expression> lhs, 
            std::unique_ptr<Expression> rhs)
    : operation_(operation)
    , lhs_(std::move(lhs))
    , rhs_(std::move(rhs)) {}

    ExprType GetExpressionType() override {
        return ExprType::kBinaryExpression;
    }

    value::ValuePtr Evaluate(executor::Context& context) override {
        const value::ValuePtr lhs = lhs_->Evaluate(context);
        const value::ValuePtr rhs = rhs_->Evaluate(context);
        switch (operation_) {
            case OperationType::kPlusOp: return value::Add(lhs, rhs);
            case OperationType::kMinusOp: return value::Subtract(lhs, rhs);
            case OperationType::kMulOp: return value::Multiply(lhs, rhs);
            case OperationType::kDivOp: return value::Divide(lhs, rhs);
            case OperationType::kRemainderOp: return value::Remainder(lhs, rhs);
            case OperationType::kLogicalAnd: return value::LogicalAnd(lhs, rhs);
            case OperationType::kLogicalOr: return value::LogicalOr(lhs, rhs);
            case OperationType::kLess: return value::Less(lhs, rhs);
            case OperationType::kGreater: return value::Greater(lhs, rhs);
            case OperationType::kLessOrEqual: return value::LessOrEqual(lhs, rhs);
            case OperationType::kGreaterOrEqual: return value::GreaterOrEqual(lhs, rhs);
            case OperationType::kEqual: return value::Equals(lhs, rhs);
            case OperationType::kNotEqual: return value::NotEquals(lhs, rhs);
            // default: throw std::runtime_error("Unknown operation");
        }
    }

private:
    OperationType operation_;
    std::unique_ptr<Expression> lhs_;
    std::unique_ptr<Expression> rhs_;
};

} // namespace itmo_script::ast