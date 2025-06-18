#include "include/parser/TokenSource.h"
#include "src/parser/headers/Token.h"

#include <stdexcept>

bool TokenSource::Match(TokenType type) {
    if (Peek().Type() == type) {
        Get();
        return true;
    }
    return false;
}

void TokenSource::Check(TokenType type) {
    if (!Match(type)) {
        throw std::runtime_error("Expected token " + token_to_string(type));
    }
}

// =============================================

VectorOwningTokenSource::VectorOwningTokenSource(std::vector<Token>&& tokens)
    : tokens_(std::move(tokens)), index_(0) {}

const Token& VectorOwningTokenSource::Peek() {
    return tokens_[index_];
}

const Token& VectorOwningTokenSource::Peek(size_t add) {
    return tokens_[index_ + add];
}

const Token& VectorOwningTokenSource::Get() {
    return tokens_[index_++];
}

bool VectorOwningTokenSource::eof() const {
    return index_ == tokens_.size();
}

// =============================================

VectorReferenceTokenSource::VectorReferenceTokenSource(const std::vector<Token>& tokens)
    : tokens_(tokens), index_(0) {}

const Token& VectorReferenceTokenSource::Peek() {
    return tokens_[index_];
}

const Token& VectorReferenceTokenSource::Peek(size_t add) {
    return tokens_[index_ + add];
}

const Token& VectorReferenceTokenSource::Get() {
    return tokens_[index_++];
}

bool VectorReferenceTokenSource::eof() const {
    return index_ == tokens_.size();
}
