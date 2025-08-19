#include "Lexer.h"


namespace isp = itmo_script::parser;

isp::Lexer::Lexer(std::istream& stream)
    : code_source_(stream)
    , location({1, 1}) {}

std::vector<isp::Token> isp::Lexer::Tokenize() {
    tokens_.clear();
    while (!code_source_.eof()) {
        char current = code_source_.peek();
        if (std::isspace(current)) {
            if (current == '\n') {
                location.line++;
                location.column = 1;
            } else {
                location.column++;
            }
            code_source_.get();
            continue;
        }
        if (std::isdigit(current)) {
            TokenizeNumber();
        } else if (kOperatorTable.contains(std::string{current}) || current == '!') {
            TokenizeOperator();
        } else if (IsLetterOrLowLine(current)) {
            TokenizeIdentifier();
        } else if (current == '"') {
            TokenizeString();
        } else if (!code_source_.eof()) {
            throw std::runtime_error(
                "Syntax error at line " + std::to_string(location.line) +
                ", column " + std::to_string(location.column) +
                ": unexpected token '" + std::string(1, current) + "'"
            );
        }
    }
    AddToken(TokenType::kEOF);
    return std::move(tokens_);
}

void isp::Lexer::TokenizeNumber() {
    std::string number;
    number.reserve(32);

    bool point = false, exponent = false;

    while (true) {
        char c = code_source_.peek();
        if (c == '.') {
            if (point) {
                throw std::runtime_error("Wrong number format: " + number);
            }
            point = true;
        } else if (c == 'e' || c == 'E') {
            if (exponent) {
                throw std::runtime_error("Wrong number format: " + number);
            }
            exponent = true;
        } else if ((c == '-' || c == '+') &&
                 !number.empty() && (number.back() == 'e' || number.back() == 'E')) {
            // allowed sign after exponent
        } else if (!std::isdigit(static_cast<unsigned char>(c))) {
            break;
        }

        number.push_back(c);
        code_source_.get();
    }

    if (number.empty() ||
        number.back() == 'e' || number.back() == 'E' ||
        number.back() == '+' || number.back() == '-' ||
        number.back() == '.') {
        throw std::runtime_error("Number ends abruptly: " + number);
    }

    AddToken(TokenType::kNumber, std::move(number));
}

void isp::Lexer::TokenizeOperator() {
    char current = code_source_.get();
    if (current == '/' && code_source_.peek() == '/') {
        // comment (yes, it's not operator :) )
        while (code_source_.peek() != '\n') {
            code_source_.get();
        }
        code_source_.get();
        return;
    }
    std::string operator_str = std::string{current};
    while (kOperatorTable.contains(operator_str + std::string{static_cast<char>(code_source_.peek())})) {
        operator_str += std::string{static_cast<char>(code_source_.get())};
    }
    AddToken(kOperatorTable.at(operator_str));
}

void isp::Lexer::TokenizeIdentifier() {
    std::string name;
    while (true) {
        char current = code_source_.peek();
        if (!(std::isdigit(current) || IsLetterOrLowLine(current))) {
            break;
        }
        name += current;
        code_source_.get();
    }
    if (kKeywordsTable.contains(name)) {
        return AddToken(kKeywordsTable.at(name));
    }
    AddToken(TokenType::kIdentifier, name);
}

void isp::Lexer::TokenizeString() {
    std::string str;
    code_source_.get();

    while (true) {
        char c = code_source_.get();
        if (c == '"') {
            break;
        }
        if (c == '\\') {
            char next = code_source_.get();
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

void isp::Lexer::AddToken(TokenType type, std::string raw_text) {
    tokens_.emplace_back(Token(type, std::move(raw_text), location));
}

void isp::Lexer::AddToken(TokenType type) {
    tokens_.emplace_back(Token(type, "", location));
}

bool isp::Lexer::IsLetterOrLowLine(char current) {
    return (
        ('a' <= current && current <= 'z') ||
        ('A' <= current && current <= 'Z') ||
        (current == '_')
    );
}