#pragma once


namespace dont_use_script::ast {

enum class OperationType {
    kPlusOp,
    kMinusOp,
    kMulOp,
    kDivOp,
    kRemainderOp,
    kPowerOp,
    kNoOp,

    kLogicalAnd,
    kLogicalOr,
    kLogicalNot,
    kEqual,
    kNotEqual,
    kLess,
    kGreater,
    kLessOrEqual,
    kGreaterOrEqual,
};

}