#pragma once

#include <string>

#include "TokenType.h"
#include "Location.h"

namespace dont_use_script::parser {

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

} // namespace dont_use_script::parser