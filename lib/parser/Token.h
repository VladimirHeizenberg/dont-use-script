#pragma once

#include <string>

enum TokenType {
    kIdentifier,

    kNumber,

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
        case kIdentifier:     return os << "kIdentifier";
        case kNumber:         return os << "kNumber";
        case kAssign:         return os << "kAssign";
        case kPlus:           return os << "kPlus";
        case kMinus:          return os << "kMinus";
        case kMul:            return os << "kMul";
        case kDiv:            return os << "kDiv";
        case kLParenthesis:   return os << "kLParenthesis";
        case kRParenthesis:   return os << "kRParenthesis";
        case kEOF:            return os << "kEOF";
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
