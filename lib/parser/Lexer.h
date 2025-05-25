#pragma once

#include <string>
#include <istream>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <utility>

#include "Token.h"
#include "AST/AST.h"

class Lexer {
public:
    Lexer(std::istream& char_source)
    : char_source_(char_source)
    , tokens(std::vector<Token>()) {}

    std::vector<Token> tokenize() {
        while(!char_source_.eof()) {
            char current = char_source_.peek();
            if (IsDigit(current)) TokenizeNumber();
            else if (IsOperator(current)) TokenizeOperator();
            else char_source_.get(); // skip
        }
        AddToken(TokenType::kEOF);
        return std::move(tokens);
    }


private:
    void TokenizeNumber() {
        std::string number;
        while (true) {
            char current = char_source_.peek();
            if (!IsDigit(current)) {
                break;
            }
            number.push_back(current);
            char_source_.get();
        }
        AddToken(TokenType::kNumber, number);
    }

    void TokenizeOperator() {
        char current = char_source_.get();
        AddToken(kOperatorTable.at(std::string{current}));
    }

    void AddToken(TokenType type) {
        AddToken(type, "");
    }

    void AddToken(TokenType type, const std::string& text) {
        tokens.emplace_back(type, text);
    }

    bool IsDigit(char current) {
        return std::isdigit(current);
    }

    bool IsOperator(char current) {
        return kOperatorTable.contains(std::string{current});
    }

    std::istream& char_source_;
    std::vector<Token> tokens;
    inline static const std::unordered_map<std::string, TokenType> kOperatorTable = {
        {"+", TokenType::kPlus},
        {"-", TokenType::kMinus},
        {"*", TokenType::kMul},
        {"/", TokenType::kDiv},
    };
};