#include "include/parser/Lexer.h"

#include <iostream>

Lexer::Lexer(std::istream &char_source)
: char_source_(char_source)
, tokens_(std::vector<Token>()) {}


std::vector<Token> Lexer::tokenize() {
    tokens_.clear();
    while(!char_source_.eof()) {
        char current = char_source_.peek();
        if (IsDigit(current) || current == '.') TokenizeNumber();
        else if (IsOperator(current) || current == '!') TokenizeOperator();
        else if (IsLetterOrLowLine(current)) TokenizeIdentifier();
        else if (current == '"') TokenizeString();
        else if (std::isspace(current)) char_source_.get(); // skip
        else if (!char_source_.eof()) {
            std::cout << char_source_.eof();
            throw std::runtime_error("Lexer: unexpected token: " + std::string(1, current));
        }
    }
    AddToken(TokenType::kEOF);
    return std::move(tokens_);
}

void Lexer::TokenizeNumber() {
    std::string number;
    bool point = false, exponent = false;
    while (true) {
        char current = char_source_.peek();
        if (current == '.') {
            if (!point) {
                point = true;
            } else {
                throw std::runtime_error("Wrong number format");
            }
        } else if (current == 'e' || current == 'E') {
            if (!exponent) {
                exponent= true;
            } else {
                throw std::runtime_error("Wrong number format");
            }
        } else if (
            (current == '-' || current == '+') &&
            (*number.rbegin() == 'e' || *number.rbegin() == 'E')) {
        } else if (!IsDigit(current)) {
            break;
        }
        number.push_back(current);
        char_source_.get();
    }
    if (number.back() == 'e' || number.back() == 'E' ||
        number.back() == '+' || number.back() == '-' ||
        number.back() == '.') {
            throw std::runtime_error("Number ends abruptly: " + number);
    }
    AddToken(TokenType::kNumber, number);
}

void Lexer::TokenizeOperator() {
    char current = char_source_.get();
    if (current == '/' && char_source_.peek() == '/') {
        // comment (yes, it's not operator :) )
        while (char_source_.peek() != '\n') {
            char_source_.get();
        }
        char_source_.get();
        return;
    }
    std::string operator_str = std::string{current};
    while (kOperatorTable.contains(operator_str + std::string{static_cast<char>(char_source_.peek())})) {
        operator_str += std::string{static_cast<char>(char_source_.get())};
    }
    AddToken(kOperatorTable.at(operator_str));
}

void Lexer::TokenizeIdentifier() {
    std::string name;
    while (true) {
        char current = char_source_.peek();
        if (!(IsDigit(current) || IsLetterOrLowLine(current))) {
            break;
        }
        name += current;
        char_source_.get();
    }
    if (kKeywordsTable.contains(name)) {
        return AddToken(kKeywordsTable.at(name));
    }
    AddToken(TokenType::kIdentifier, name);
}

void Lexer::TokenizeString() {
    std::string str;
    char_source_.get();

    while (true) {
        char c = char_source_.get();
        if (c == '"') {
            break;
        }
        if (c == '\\') {
            char next = char_source_.get();
            if (kEscapeSequences.contains(next)) {
                str += kEscapeSequences.at(next);
            } else {
                throw std::runtime_error("Wrong escape sequence in string");
            }
        } else {
            str += c;
        }
    }

    AddToken(TokenType::kString, str);
}

void Lexer::AddToken(TokenType type) {
    AddToken(type, "");
}

void Lexer::AddToken(TokenType type, const std::string& text) {
    tokens_.emplace_back(type, text);
}


bool Lexer::IsDigit(char current) {
    return std::isdigit(current);
}

bool Lexer::IsOperator(char current) {
    return kOperatorTable.contains(std::string{current});
}

bool Lexer::IsLetterOrLowLine(char current) {
    return (
        ('a' <= current && current <= 'z') ||
        ('A' <= current && current <= 'Z') ||
        (current == '_')
    );
}