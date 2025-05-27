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
    kIf,
    kThen,
    kElse,
    
    // operators
    kAssign, // =
    kPlus, // +
    kMinus, // -
    kMul, // *
    kDiv, // /

    kLParenthesis, // (
    kRParenthesis, // )

    kEnd,
    kEOF,
};

std::ostream& operator<<(std::ostream& os, TokenType type) {
    switch (type) {
        case TokenType::kIdentifier:     return os << "kIdentifier";
        case TokenType::kNumber:         return os << "kNumber";
        case TokenType::kPrint:          return os << "kPrint";
        case TokenType::kPrintln:        return os << "kPrintln";
        case TokenType::kTrue:           return os << "kTrue";
        case TokenType::kFalse:          return os << "kFalse";
        case TokenType::kIf:             return os << "kIf";
        case TokenType::kThen:           return os << "kThen";
        case TokenType::kElse:           return os << "kElse";
        case TokenType::kAssign:         return os << "kAssign";
        case TokenType::kPlus:           return os << "kPlus";
        case TokenType::kMinus:          return os << "kMinus";
        case TokenType::kMul:            return os << "kMul";
        case TokenType::kDiv:            return os << "kDiv";
        case TokenType::kLParenthesis:   return os << "kLParenthesis";
        case TokenType::kRParenthesis:   return os << "kRParenthesis";
        case TokenType::kEnd:            return os << "kEnd";
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
