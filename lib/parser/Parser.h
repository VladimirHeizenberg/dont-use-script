#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "AST/AST.h"
#include "Token.h"

class Parser {
public:
    using expression = std::unique_ptr<ExpressionAST>;
    Parser(std::vector<Token> tokens)
    : tokens_(std::move(tokens))
    , position_(0) {}

    std::vector<expression> parse() {
        std::vector<expression> result;
        while (!Match(TokenType::kEOF)) {
            result.push_back(ParseExpression());
        }
        return result;
    }
private:
    expression ParseExpression() {
        return ParseAdd();
    }
    expression ParseAdd() {
        expression expr = ParseMult();
        while (true) {
            if (Match(TokenType::kPlus)) {
                expr = std::make_unique<BinaryExpressionAST>(
                    OperationType::kPlusOp, std::move(expr), ParseMult()
                );
            } else if (Match(TokenType::kMinus)) {
                expr = std::make_unique<BinaryExpressionAST>(
                    OperationType::kMinusOp, std::move(expr), ParseMult()
                );
            } else {
                break;
            }
        }
        return expr;
    }

    expression ParseMult() {
        expression expr = ParseUnary();
        while (true) {
            if (Match(TokenType::kMul)) {
                expr = std::make_unique<BinaryExpressionAST>(
                    OperationType::kMulOp, std::move(expr), ParseUnary()
                );
            } else if (Match(TokenType::kDiv)) {
                expr = std::make_unique<BinaryExpressionAST>(
                    OperationType::kDivOp, std::move(expr), ParseUnary()
                );
            } else {
                break;
            }
        }
        return expr;
    }

    expression ParseUnary() {
        return ParseLiteral();
    }

    expression ParseLiteral() {
        auto token = get();
        if (Match(TokenType::kNumber)) {
            return std::make_unique<ConstExpressionAST>(
                std::stod(token.text())
            );
        }
    }

    bool Match(TokenType type) {
        bool ans = (get().type() == type);
        if (ans) {
            ++position_;
            return true;
        }
        return false;
    }

    Token get() {
        if (position_ > tokens_.size()) {
            return Token(TokenType::kEOF, "");
        }
        return tokens_[position_];
    }

    size_t position_;
    std::vector<Token> tokens_;
};