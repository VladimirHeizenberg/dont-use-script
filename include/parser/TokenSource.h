#pragma once

#include <vector>

#include "src/parser/headers/Token.h"


class TokenSource {
public:
    virtual const Token& Peek() = 0;
    virtual const Token& Peek(size_t add) = 0;
    virtual const Token& Get() = 0;
    [[nodiscard]] virtual bool eof() const = 0;

    bool Match(TokenType type);
    void Check(TokenType type);

    virtual ~TokenSource() = default;
};

class VectorOwningTokenSource : public TokenSource {
public:
    VectorOwningTokenSource(std::vector<Token>&& tokens);

    const Token& Peek() override;
    const Token& Peek(size_t add) override;
    const Token& Get() override;
    [[nodiscard]] bool eof() const override;

private:
    std::vector<Token> tokens_;
    size_t index_;
};

class VectorReferenceTokenSource : public TokenSource {
public:
    VectorReferenceTokenSource(const std::vector<Token>& tokens);

    const Token& Peek() override;
    const Token& Peek(size_t add) override;
    const Token& Get() override;
    [[nodiscard]] bool eof() const override;

private:
    const std::vector<Token>& tokens_;
    size_t index_;
};
