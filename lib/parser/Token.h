#pragma once

#include <string>

enum class TokenType {
    kIdentifier,

    kNumber,

    // keywords
    kPrint,
    kPrintln,
    kTrue,
    kFalse,


    // operators
    kAssign, // =
    kPlus, // +
    kMinus, // -
    kMul, // *
    kDiv, // /

    kLParenthesis, // (
    kRParenthesis, // )

    kEOF,
};

std::ostream& operator<<(std::ostream& os, TokenType type) {
    switch (type) {
        case TokenType::kIdentifier:     return os << "kIdentifier";
        case TokenType::kNumber:         return os << "kNumber";
        case TokenType::kPrint:          return os << "kPrint";
        case TokenType::kPrintln:        return os << "kPrintln";
        case TokenType::kAssign:         return os << "kAssign";
        case TokenType::kPlus:           return os << "kPlus";
        case TokenType::kMinus:          return os << "kMinus";
        case TokenType::kMul:            return os << "kMul";
        case TokenType::kDiv:            return os << "kDiv";
        case TokenType::kLParenthesis:   return os << "kLParenthesis";
        case TokenType::kRParenthesis:   return os << "kRParenthesis";
        case TokenType::kEOF:            return os << "kEOF";
        default:              return os << "Unknown TokenType";
    }
}

class Token {
public:
    Token() = default;
    Token(TokenType type, const std::string& text)
    : type_(type)
    , text_(text) {}

    TokenType type() {
        return type_;
    }

    const std::string& text() {
        return text_;
    }

private:
    TokenType type_;
    std::string text_;
};
