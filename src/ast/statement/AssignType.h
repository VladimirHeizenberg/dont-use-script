#pragma once


namespace itmo_script::ast {

enum class AssignType {
    kAssign,
    kPlusAssign,
    kMinusAssign,
    kMulAssign,
    kDivAssign,
};

} // namespace itmo_script::ast