#pragma once

#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <utility>

#include "Token.h"
#include "AST/AST.h"
#include "CharSourse.h"

class Lexer {
public:
    Lexer(std::unique_ptr<CharSource>&& char_source)
    : char_source_(std::move(char_source))
    , tokens(std::vector<Token>()) {}

    std::vector<Token> tokenize() {
        while(!char_source_->eof()) {
            char current = char_source_->peek();
            if (IsDigit(current) || current == '.') TokenizeNumber();
            else if (IsOperator(current) || current == '!') TokenizeOperator();
            else if (IsLetterOrLowLine(current)) TokenizeIdentifier();
            else if (current == '"') TokenizeString();
            else char_source_->get(); // skip
        }
        AddToken(TokenType::kEOF);
        return std::move(tokens);
    }


private:
    void TokenizeNumber() {
        std::string number;
        bool point = false, exponent = false;
        while (true) {
            char current = char_source_->peek();
            if (current == '.') {
                if (!point) {
                    point = true;
                } else {
                    throw std::runtime_error("Wrong number format");
                }
            }
            else if (current == 'e' || current == 'E') {
                if (!exponent) {
                    exponent= true;
                } else {
                    throw std::runtime_error("Wrong number format");
                }
            }
            else if (
                (current == '-' || current == '+') &&
                (*number.rbegin() == 'e' || *number.rbegin() == 'E')
                ) {}
            else if (!IsDigit(current)) {
                break;
            }
            number.push_back(current);
            char_source_->get();
        }
        if (number.back() == 'e' || number.back() == 'E' ||
            number.back() == '+' || number.back() == '-' ||
            number.back() == '.') {
            throw std::runtime_error("Number ends abruptly: " + number);
        }
        AddToken(TokenType::kNumber, number);
    }

    void TokenizeOperator() {
        char current = char_source_->get();
        std::string operator_str = std::string{current};
        while (kOperatorTable.contains(operator_str + std::string{(char)char_source_->peek()})) {
            operator_str += std::string{(char)char_source_->get()};
        }
        AddToken(kOperatorTable.at(operator_str));
    }

    void TokenizeIdentifier() {
        std::string name;
        while (true) {
            char current = char_source_->peek();
            if (!(IsDigit(current) || IsLetterOrLowLine(current))) {
                break;
            }
            name += current;
            char_source_->get();
        }
        if (kKeywordsTable.contains(name)) {
            return AddToken(kKeywordsTable.at(name));
        }
        AddToken(TokenType::kIdentifier, name);
    }

    void TokenizeString() {
        std::string str;
        char_source_->get(); // eat "
        while (char_source_->peek() != '"') {
            str += char_source_->get();
        }
        char_source_->get(); // eat "
        AddToken(TokenType::kString, str);
    }

    void AddToken(TokenType type) {
        AddToken(type, "");
    }

    void AddToken(TokenType type, const std::string& text) {
        tokens.emplace_back(type, text);
    }

    static bool IsDigit(char current) {
        return std::isdigit(current);
    }

    static bool IsOperator(char current) {
        return kOperatorTable.contains(std::string{current});
    }

    static bool IsLetterOrLowLine(char current) {
        return (
            ('a' <= current && current <= 'z') ||
            ('A' <= current && current <= 'Z') ||
            (current == '_')
        );
    }

    std::unique_ptr<CharSource> char_source_;
    std::vector<Token> tokens;
    inline static const std::unordered_map<std::string, TokenType> kOperatorTable = {
        {"+",   TokenType::kPlus},
        {"-",   TokenType::kMinus},
        {"*",   TokenType::kMul},
        {"/",   TokenType::kDiv},
        {"=",   TokenType::kAssign},
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
    };

    inline static const std::unordered_map<std::string, TokenType> kKeywordsTable = {
        {"print",   TokenType::kPrint},
        {"println", TokenType::kPrintln},
        {"true",    TokenType::kTrue},
        {"false",   TokenType::kFalse},
        {"if",      TokenType::kIf},
        {"elif",    TokenType::kElif},
        {"then",    TokenType::kThen},
        {"else",    TokenType::kElse},
        {"end",     TokenType::kEnd},

        {"while",   TokenType::kWhile},

        // special words still this is operators
        {"and",     TokenType::kLogicalAnd},
        {"or",      TokenType::kLogicalOr},
        {"not",     TokenType::kLogicalNot},
    };
};