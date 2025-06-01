#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <functional>

#include "AST/AST.h"
#include "Token.h"
#include "TokenSource.h"

class Parser {
public:
    using expression            = std::unique_ptr<ExpressionAST>;
    using statement             = std::unique_ptr<StatementAST>;
    using token_and_op          = std::pair<TokenType, OperationType>;
    using operators_table       = const std::vector<token_and_op>;
    using token_and_assign      = std::pair<TokenType, AssignmentOperationType>;
    using assignment_table      = const std::vector<token_and_assign>;

    Parser(std::unique_ptr<TokenSource> tokens, VariablesTable& table, std::ostream& out)
    : tokens_(std::move(tokens))
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
        if (Match(TokenType::kWhile)) return ParseWhileStatement();
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

    statement ParseWhileStatement() {
        expression while_expression = ParseExpression();
        statement while_statement = ParseScopeStatement();
        Check(TokenType::kEnd);
        Check(TokenType::kWhile);
        return std::make_unique<WhileStatement>(
            std::move(while_expression), std::move(while_statement)
        );
    }

    statement ParseScopeStatement() {
        std::unique_ptr<ScopeStatement> scope = std::make_unique<ScopeStatement>();
        while (Peek().Type() != TokenType::kElse && Peek().Type() != TokenType::kEnd) {
            scope->add(ParseStatement());
        }
        return scope;
    }

    statement ParseAssignStatement() {
        for (auto& [token, assignment]: assignment_table_) {
            // std::cerr << Peek().Type() << " " << Get(1).Type() << "\n";
            if (Peek().Type() == TokenType::kIdentifier && Peek(1).Type() == token) {
                Token current = Peek();
                Match(TokenType::kIdentifier);
                Check(token);
                return std::make_unique<AssignStatement>(
                    current.Text(), ParseExpression(), variables_table_, assignment
                );
            }
        }
        throw std::runtime_error("not now");
    }

    template <typename NextPriority>
    expression ParseBinaryOperations(NextPriority next_level, operators_table& table) {
        expression expr = (this->*next_level)();
        bool matched = false;
        while (true) {
            matched = false;
            for (const auto& [token, operation] : table) {
                if (Match(token)) {
                    expr = std::make_unique<BinaryExpressionAST>(
                        operation, std::move(expr), (this->*next_level)()
                    );
                    matched = true;
                    break;
                }
            }
            if (!matched) break;
        }
        return expr;
    }


    expression ParseExpression() {
        return ParseLogicalOr();
    }

    expression ParseLogicalOr() {
        return ParseBinaryOperations(&Parser::ParseLogicalAnd, logical_or_table_);
    }

    expression ParseLogicalAnd() {
        return ParseBinaryOperations(&Parser::ParseEqual, logical_and_table_);
    }

    expression ParseEqual() {
        return ParseBinaryOperations(&Parser::ParseRelation, equality_table_);
    }

    expression ParseRelation() {
        return ParseBinaryOperations(&Parser::ParseAdd, relations_table_);
    }

    expression ParseAdd() {
        return ParseBinaryOperations(&Parser::ParseMult, add_table_);
    }

    expression ParseMult() {
        return ParseBinaryOperations(&Parser::ParseUnaryPlusMinus, mult_table_);
    }

    expression ParseUnaryPlusMinus() {
        if (Match(TokenType::kMinus)) {
            return std::make_unique<UnaryExpressionAST>(
                OperationType::kMinusOp, ParseLogicalNot()
            );
        }
        if (Match(TokenType::kPlus)) {
            return std::make_unique<UnaryExpressionAST>(
                OperationType::kPlusOp, ParseLogicalNot()
            );
        }
        return std::make_unique<UnaryExpressionAST>(
            OperationType::kNoOp, ParseLogicalNot()
        );
    }

    expression ParseLogicalNot() {
        if (Match(TokenType::kLogicalNot)) {
            return std::make_unique<UnaryExpressionAST>(
                OperationType::kLogicalNot, ParseLiteral()
            );
        }
        return ParseLiteral();
    }

    expression ParseLiteral() {
        auto token = Peek();
        // TODO: replace with the map
        if (Match(TokenType::kNumber)) {
            return std::make_unique<ConstExpressionAST>(
                std::stod(token.Text())
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
                token.Text(), variables_table_
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
        return tokens_->Match(type);
    }

    void Check(TokenType type) {
        tokens_->Check(type);
    }

    const Token& Peek(size_t additional = 0) {
        return tokens_->Peek(additional);
    }

    std::unique_ptr<TokenSource> tokens_;
    VariablesTable& variables_table_;
    std::ostream& out_;

    // tables for translation from tokentype to operations

    static inline operators_table logical_or_table_ = {
         {TokenType::kLogicalOr, OperationType::kLogicalOr},
    };

    static inline operators_table logical_and_table_ = {
         {TokenType::kLogicalAnd, OperationType::kLogicalAnd},
    };

    static inline operators_table equality_table_ = {
        {TokenType::kEqual,     OperationType::kEqual},
        {TokenType::kNotEqual,  OperationType::kNotEqual},
    };

    static inline operators_table relations_table_ = {
        {TokenType::kLess,            OperationType::kLess},
        {TokenType::kGreater,         OperationType::kGreater},
        {TokenType::kLessOrEqual,     OperationType::kLessOrEqual},
        {TokenType::kGreaterOrEqual,  OperationType::kGreaterOrEqual},
    };

    static inline operators_table add_table_ = {
        {TokenType::kPlus,   OperationType::kPlusOp},
        {TokenType::kMinus,  OperationType::kMinusOp},
    };

    static inline operators_table mult_table_ = {
        {TokenType::kMul, OperationType::kMulOp},
        {TokenType::kDiv, OperationType::kDivOp},
    };

    static inline assignment_table assignment_table_ = {
        {TokenType::kAssign,        AssignmentOperationType::kAssign},
        {TokenType::kPlusAssign,    AssignmentOperationType::kPlusAssign},
        {TokenType::kMinusAssign,   AssignmentOperationType::kMinusAssign},
        {TokenType::kMulAssign,     AssignmentOperationType::kMulAssign},
        {TokenType::kDivAssign,     AssignmentOperationType::kDivAssign},
    };
};