#pragma once

#include <memory>
#include <stdexcept>

#include "Expression.h"
#include "OperationType.h"
#include "value/MakeValue.h"
#include "value/Operation.h"



namespace dont_use_script::ast {

class UnaryExpr: public Expression {
public:
    UnaryExpr(OperationType operation, 
              std::unique_ptr<Expression> expr)
    : operation_(operation)
    , expr_(std::move(expr)) {}

    ExprType GetExpressionType() override {
        return ExprType::kUnaryExpression;
    }

    value::ValuePtr Evaluate(executor::Context& context) override {
        value::ValuePtr result = expr_->Evaluate(context);
        switch (operation_) {
        case OperationType::kNoOp:
            return result;
        case OperationType::kPlusOp: 
            if (result->GetValueType() == value::ValueType::kDoubleValue)
                return result;
            throw std::runtime_error("Wrong type for unary expression +\n");
        case OperationType::kMinusOp: 
            if (result->GetValueType() == value::ValueType::kDoubleValue) {
                return value::MakeDouble(-result->AsDouble());
            }
            throw std::runtime_error("Wrong type for unary expression -\n");
        case OperationType::kLogicalNot:
            return value::LogicalNot(result);
        default:
            throw std::runtime_error("Wrong unary operator");
        }
    }
private:
    OperationType operation_;
    std::unique_ptr<Expression> expr_;
};

} // namespace dont_use_script::ast