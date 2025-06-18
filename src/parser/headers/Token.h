#pragma once

#include <string>

enum class TokenType {
    kIdentifier,

    kNumber,
    kString,

    // keywords
    kPrint,
    kPrintln,
    kTrue,
    kFalse,
    kIf,
    kThen,
    kElif,
    kElse,
    kFor,
    kWhile,
    kBreak,
    kContinue,
    kFunction,
    kReturn,
    kIn,

    kNullType,

    // operators
    kAssign, // =
    kPlus, // +
    kMinus, // -
    kMul, // *
    kDiv, // /
    kRemainder, // %
    kPower, // ^
    kPlusAssign, // +=
    kMinusAssign, // -=
    kMulAssign, // *=
    kDivAssign, // /=
    kEqual, // ==
    kNotEqual, // !=
    kLess, // <
    kGreater, // >
    kLessOrEqual, // <=
    kGreaterOrEqual, // >=
    kLogicalAnd, // and
    kLogicalOr, // or
    kLogicalNot, // not
    kColon, // :

    // Punctuation
    kLParenthesis, // (
    kRParenthesis, // )
    kLBracket,
    kRBracket,
    kComma, // ,

    kEnd,
    kEOF,
};

std::string token_to_string(TokenType token);

class Token {
public:
    Token() = delete;
    Token(TokenType type, std::string text);
    [[nodiscard]] TokenType Type() const;
    [[nodiscard]] const std::string& Text() const;

private:
    TokenType type_;
    std::string text_;
};
