#include "src/parser/headers/Token.h"

#include <unordered_map>

Token::Token(TokenType type, std::string text)
    : type_(type), text_(std::move(text)) {}


const std::string& Token::Text() const {
    return text_;
}

TokenType Token::Type() const {
    return type_;
}



std::string token_to_string(TokenType token) {
    static const std::unordered_map<TokenType, std::string> token_map = {
        {TokenType::kIdentifier, "identifier"},
        {TokenType::kNumber, "number"},
        {TokenType::kString, "string"},

        // keywords
        {TokenType::kPrint, "print"},
        {TokenType::kPrintln, "println"},
        {TokenType::kTrue, "true"},
        {TokenType::kFalse, "false"},
        {TokenType::kIf, "if"},
        {TokenType::kThen, "then"},
        {TokenType::kElif, "elif"},
        {TokenType::kElse, "else"},
        {TokenType::kWhile, "while"},
        {TokenType::kBreak, "break"},
        {TokenType::kContinue, "continue"},
        {TokenType::kFunction, "function"},
        {TokenType::kReturn, "return"},

        // operators (use symbol from comments)
        {TokenType::kAssign, "="},
        {TokenType::kPlus, "+"},
        {TokenType::kMinus, "-"},
        {TokenType::kMul, "*"},
        {TokenType::kDiv, "/"},
        {TokenType::kRemainder, "%"},
        {TokenType::kPower, "^"},
        {TokenType::kPlusAssign, "+="},
        {TokenType::kMinusAssign, "-="},
        {TokenType::kMulAssign, "*="},
        {TokenType::kDivAssign, "/="},
        {TokenType::kEqual, "=="},
        {TokenType::kNotEqual, "!="},
        {TokenType::kLess, "<"},
        {TokenType::kGreater, ">"},
        {TokenType::kLessOrEqual, "<="},
        {TokenType::kGreaterOrEqual, ">="},
        {TokenType::kLogicalAnd, "and"},
        {TokenType::kLogicalOr, "or"},
        {TokenType::kLogicalNot, "not"},

        // punctuation
        {TokenType::kLParenthesis, "("},
        {TokenType::kRParenthesis, ")"},
        {TokenType::kComma, ","},

        {TokenType::kEnd, "end"},
        {TokenType::kEOF, "eof"},
    };

    auto it = token_map.find(token);
    if (it != token_map.end()) {
        return it->second;
    }
    return "unknown";
}