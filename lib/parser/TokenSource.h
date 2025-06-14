#pragma once

#include <vector>
#include <stdexcept>

#include "Token.h"

class TokenSource {
public:
    virtual const Token& Peek() = 0;
    virtual const Token& Peek(size_t add) = 0;
    virtual const Token& Get() = 0;
    [[nodiscard]] virtual bool eof() const = 0;

    bool Match(TokenType type) {
        if (Peek().Type() == type) {
            Get();
            return true;
        }
        return false;
    }

    void Check(TokenType type) {
        if (!Match(type)) {
            throw std::runtime_error("Expected token " + token_to_string(type));
        }
    }

    virtual ~TokenSource() = default;
};

class VectorOwningTokenSource: public TokenSource {
public:
    VectorOwningTokenSource(std::vector<Token>&& tokens)
    : tokens_(std::move(tokens))
    , index_(0) {}

    const Token& Peek() override {
        return tokens_[index_];
    }

    const Token& Peek(size_t add) override {
        return tokens_[index_ + add];
    }

    const Token& Get() override {
        return tokens_[index_++];
    }

    [[nodiscard]] bool eof() const override {
        return index_ == tokens_.size();
    }
private:
    std::vector<Token> tokens_;
    size_t index_;
};

class VectorReferenceTokenSource: public TokenSource {
public:
    VectorReferenceTokenSource(const std::vector<Token>& tokens)
    : tokens_(tokens)
    , index_(0) {}

    const Token& Peek() override {
        return tokens_[index_];
    }

    const Token& Peek(size_t add) override {
        return tokens_[index_ + add];
    }

    const Token& Get() override {
        return tokens_[index_++];
    }

    [[nodiscard]] bool eof() const override {
        return index_ == tokens_.size();
    }
private:
    const std::vector<Token>& tokens_;
    size_t index_;
};