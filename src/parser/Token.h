#pragma once

#include <string>

#include "TokenType.h"


namespace itmo_script::parser {

struct Location {
    size_t line;
    size_t column;
};

class Token {
public:
    Token(TokenType type, std::string raw_text, Location location) 
    : type_(type)
    , raw_text_(raw_text)
    , location_(location) {}

    [[nodiscard]] TokenType GetType() const {
        return type_;
    }

    [[nodiscard]] const std::string& GetText() const {
        return raw_text_;
    }

    [[nodiscard]] Location GetLocation() const {
        return location_;
    }

private:
    TokenType type_;
    std::string raw_text_; // TODO: think about how to make it more memory-friendly
    Location location_;
};

} // namespace itmo_script::parser