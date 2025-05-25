#pragma once

#include <string>

enum TokenType {
    kNumber,

    kPlus, // +
    kMinus, // -
    kMul, // *
    kDiv, // /

    kEOF,
};

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
