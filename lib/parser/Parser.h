#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <functional>

#include "AST/AST.h"
#include "Token.h"

class Parser {
public:
    using expression    = std::unique_ptr<ExpressionAST>;
    using statement     = std::unique_ptr<StatementAST>;

    Parser(std::vector<Token> tokens, VariablesTable& table, std::ostream& out)
    : tokens_(std::move(tokens))
    , position_(0)
    , variables_table_(table)
    , out_(out) {}

    std::vector<statement> parse() {
        std::vector<statement> result;
        while (!Match(TokenType::kEOF)) {
            result.push_back(ParseStatement());
        }
        return result;
    }
private:

    statement ParseStatement() {
        if (Match(TokenType::kPrint)) return ParsePrintStatement();
        if (Match(TokenType::kPrintln)) return ParsePrintlnStatement();
        if (Match(TokenType::kIf)) return ParseIfStatement();
        return ParseAssignStatement();
    }

    statement ParsePrintStatement() {
        Check(TokenType::kLParenthesis);
        statement print = std::make_unique<PrintStatement>(
            ParseExpression(), out_
        );
        Check(TokenType::kRParenthesis);
        return print;
    }

    statement ParsePrintlnStatement() {
        Check(TokenType::kLParenthesis);
        statement println = std::make_unique<PrintlnStatement>(
            ParseExpression(), out_
        );
        Check(TokenType::kRParenthesis);
        return println;
    }

    statement ParseIfStatement() {
        std::cout << "parsing if\n";
        expression if_expression = ParseExpression();
        Check(TokenType::kThen);
        statement statement_true = ParseScopeStatement();
        statement statement_false = std::make_unique<EmptyStatement>();
        if (Match(TokenType::kElse)) {
            statement_false = ParseScopeStatement();
        }
        Check(TokenType::kEnd);
        Check(TokenType::kIf);
        return std::make_unique<IfStatement>(
            std::move(if_expression), std::move(statement_true), std::move(statement_false)
        );
    }

    statement ParseScopeStatement() {
        std::unique_ptr<ScopeStatement> scope = std::make_unique<ScopeStatement>();
        while (get().type() != TokenType::kElse && get().type() != TokenType::kEnd) {
            scope->add(ParseStatement());
        }
        return scope;
    }

    statement ParseAssignStatement() {
        if (get().type() == TokenType::kIdentifier && get(1).type() == TokenType::kAssign) {
            Token current = get();
            Match(TokenType::kIdentifier);
            Check(TokenType::kAssign);
            return std::make_unique<AssignStatement>(
                current.text(), ParseExpression(), variables_table_
            );
        }
        throw std::runtime_error("not now");
    }

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
        if (Match(TokenType::kMinus)) {
            return std::make_unique<UnaryExpressionAST>(
                OperationType::kMinusOp, ParseLiteral()
            );
        }
        if (Match(TokenType::kPlus)) {
            return std::make_unique<UnaryExpressionAST>(
                OperationType::kPlusOp, ParseLiteral()
            );
        }
        return std::make_unique<UnaryExpressionAST>(
            OperationType::kNoOp, ParseLiteral()
        );
    }

    expression ParseLiteral() {
        auto token = get();
        // TODO: replace with the map
        if (Match(TokenType::kNumber)) {
            return std::make_unique<ConstExpressionAST>(
                std::stod(token.text())
            );
        }
        if (Match(TokenType::kTrue)) {
            return std::make_unique<ConstExpressionAST>(
                Value(true)
            );
        }
        if (Match(TokenType::kFalse)) {
            return std::make_unique<ConstExpressionAST>(
                Value(false)
            );
        }
        if (Match(TokenType::kIdentifier)) {
            return std::make_unique<VariableExpression>(
                token.text(), variables_table_
            );
        }
        if (Match(TokenType::kLParenthesis)) {
            auto expr = ParseExpression();
            Check(TokenType::kRParenthesis);
            return expr;
        }
        throw std::runtime_error("bye");
    }

    bool Match(TokenType type) {
        bool ans = (get().type() == type);
        if (ans) {
            ++position_;
            return true;
        }
        return false;
    }

    void Check(TokenType type) {
        if (!Match(type)) {
            throw std::runtime_error("invalid syntax");
        }
    }

    Token get(size_t additional = 0) {
        if (position_ + additional > tokens_.size()) {
            return Token(TokenType::kEOF, "");
        }
        return tokens_[position_ + additional];
    }

    size_t position_;
    std::vector<Token> tokens_;
    VariablesTable& variables_table_;
    std::ostream& out_;

    // inline static const std::unordered_map<TokenType, std::function<std::unique_ptr<ExpressionAST>>> literals_table_ = {
    //     {TokenType::kNumber, []() {}},

    // };
};