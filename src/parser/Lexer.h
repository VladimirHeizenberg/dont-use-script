#pragma once

#include <istream>
#include <vector>
#include <unordered_map>

#include "Token.h"


namespace itmo_script::parser {

class Lexer {
public:
    Lexer(std::istream& stream);
    std::vector<Token> Tokenize();
private:
    void TokenizeNumber();
    void TokenizeOperator();
    void TokenizeIdentifier();
    void TokenizeString();

    void AddToken(TokenType, std::string);
    void AddToken(TokenType);

    static bool IsLetterOrLowLine(char current);

    std::istream& code_source_;
    std::vector<Token> tokens_;

    Location location;

    inline static const std::unordered_map<std::string, TokenType> kOperatorTable = {
        {"+",   TokenType::kPlus},
        {"-",   TokenType::kMinus},
        {"*",   TokenType::kAsteriks},
        {"/",   TokenType::kSlash},
        {"%",   TokenType::kRemainder},
        {"^",   TokenType::kPower},
        {"=",   TokenType::kAssign},
        {",",   TokenType::kComma},
        {"+=",  TokenType::kPlusAssign},
        {"-=",  TokenType::kMinusAssign},
        {"*=",  TokenType::kMulAssign},
        {"/=",  TokenType::kDivAssign},
        {"==",  TokenType::kEqual},
        {"!=",  TokenType::kNotEqual},
        {"<",   TokenType::kLess},
        {">",   TokenType::kGreater},
        {"<=",  TokenType::kLessOrEqual},
        {">=",  TokenType::kGreaterOrEqual},
        {"(",   TokenType::kLParenthesis},
        {")",   TokenType::kRParenthesis},
        {"[",   TokenType::kLBracket},
        {"]",   TokenType::kRBracket},
        {":",   TokenType::kColon},
        {";",   TokenType::kSemicolon},
    };

    inline static const std::unordered_map<std::string, TokenType> kKeywordsTable = {
        {"true",     TokenType::kTrue},
        {"false",    TokenType::kFalse},
        {"if",       TokenType::kIf},
        {"elif",     TokenType::kElif},
        {"then",     TokenType::kThen},
        {"else",     TokenType::kElse},

        {"while",    TokenType::kWhile},
        {"for",      TokenType::kFor},
        {"break",    TokenType::kBreak},
        {"continue", TokenType::kContinue},

        {"in",       TokenType::kIn},

        {"function", TokenType::kFunction},
        {"return",   TokenType::kReturn},

        {"end",      TokenType::kEnd},

        {"nil",      TokenType::kNullType},
        // keywords but operators
        {"and",      TokenType::kLogicalAnd},
        {"or",       TokenType::kLogicalOr},
        {"not",      TokenType::kLogicalNot},
    };

    inline static const std::unordered_map<char, char> kEscapeSequences = {
        {'n',   '\n'},
        {'t',   '\t'},
        {'r',   '\r'},
        {'\\',  '\\'},
        {'"',   '"' },
        {'\'',  '\''},
        {'0',   '\0'},
    };
};

} // namespace itmo_script::parser