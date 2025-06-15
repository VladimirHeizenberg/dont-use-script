#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <stdexcept>
#include <functional>
#include <set>

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

    Parser(std::unique_ptr<TokenSource> tokens)
    : tokens_(std::move(tokens)) {}

    std::vector<statement> parse() {
        std::vector<statement> result;
        while (!Match(TokenType::kEOF)) {
            result.push_back(ParseStatement(false));
        }
        return result;
    }
private:

    statement ParseStatement(bool parsing_function_flag) {
        if (Match(TokenType::kPrint)) return ParsePrintStatement();
        if (Match(TokenType::kPrintln)) return ParsePrintlnStatement();
        if (Match(TokenType::kIf)) return ParseIfStatement(true, parsing_function_flag);
        if (Match(TokenType::kWhile)) return ParseWhileStatement(parsing_function_flag);
        if (Match(TokenType::kReturn)) {
            if (!parsing_function_flag) {
                throw std::runtime_error("return outside function");
            }
            return ParseReturnStatement();
        }
        if (Match(TokenType::kBreak)) return std::make_unique<BreakStatement>();
        if (Match(TokenType::kContinue)) return std::make_unique<ContinueStatement>();
        if (Peek().Type() == TokenType::kIdentifier && Peek(1).Type() == TokenType::kAssign) {
            return ParseAssignStatement();
        }
        return std::make_unique<ExpressionStatement>(ParseExpression());
    }

    statement ParsePrintStatement() {
        Check(TokenType::kLParenthesis);
        statement print = std::make_unique<PrintStatement>(
            ParseExpression()
        );
        Check(TokenType::kRParenthesis);
        return print;
    }

    statement ParsePrintlnStatement() {
        Check(TokenType::kLParenthesis);
        statement println = std::make_unique<PrintlnStatement>(
            ParseExpression()
        );
        Check(TokenType::kRParenthesis);
        return println;
    }

    statement ParseIfStatement(bool flag, bool parsing_function_flag) {
        expression if_expression = ParseExpression();
        Check(TokenType::kThen);
        statement statement_true = ParseScopeStatement(
            {TokenType::kEnd, TokenType::kElif, TokenType::kElse},
            parsing_function_flag
        );
        statement statement_false = std::make_unique<EmptyStatement>();
        while (Match(TokenType::kElif)) {
            statement_false = ParseIfStatement(false, parsing_function_flag);
        }
        if (Match(TokenType::kElse)) {
            statement_false = ParseScopeStatement(
                {TokenType::kEnd, TokenType::kElif, TokenType::kElse},
                parsing_function_flag
            );
        }
        if (flag) {
            Check(TokenType::kEnd);
            Check(TokenType::kIf);
        }
        return std::make_unique<IfStatement>(
            std::move(if_expression), std::move(statement_true), std::move(statement_false)
        );
    }


    statement ParseWhileStatement(bool parsing_function_flag) {
        expression while_expression = ParseExpression();
        statement while_statement = ParseScopeStatement({TokenType::kEnd}, parsing_function_flag);
        Check(TokenType::kEnd);
        Check(TokenType::kWhile);
        return std::make_unique<WhileStatement>(
            std::move(while_expression), std::move(while_statement)
        );
    }

    statement ParseScopeStatement(const std::set<TokenType>& stop_words,
                                  bool parsing_function_flag) {
        std::unique_ptr<ScopeStatement> scope = std::make_unique<ScopeStatement>();
        while (!stop_words.contains(Peek().Type())) {
            scope->add(ParseStatement(parsing_function_flag));
        }
        return scope;
    }

    statement ParseAssignStatement() {
        for (auto& [token, assignment]: assignment_table_) {
            // std::cerr << Peek().Type() << " " << Get(1).Type() << "\n";
            if (Peek().Type() == TokenType::kIdentifier && Peek(1).Type() == token) {
                Token current = Peek();
                Check(TokenType::kIdentifier);
                Check(token);
                return std::make_unique<AssignStatement>(
                    current.Text(), ParseExpression(), assignment
                );
            }
        }
        throw std::runtime_error("not now");
    }

    statement ParseReturnStatement() {
        return std::make_unique<ReturnStatement>(ParseExpression());
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
        return ParseLogicalNot();
    }

    expression ParseLogicalNot() {
        if (Match(TokenType::kLogicalNot)) {
            return std::make_unique<UnaryExpressionAST>(
                OperationType::kLogicalNot, ParseSingleExpression()
            );
        }
        return ParseSingleExpression();
    }

    expression ParseSingleExpression() {
        auto expr = ParseLiteral();

        while (true) {
            if (Match(TokenType::kLParenthesis)) {
                expr = ParseFunctionCall(std::move(expr));
            } else if (Match(TokenType::kLBracket)) {

            } else break;
        }
        return expr;
    }

    expression ParseLiteral() {
        auto token = Peek();
        // TODO: replace with the map
        if (Match(TokenType::kNumber)) {
            return std::make_unique<ConstExpressionAST>(
                MakeDoubleValue(std::stod(token.Text()))
            );
        }
        if (Match(TokenType::kString)) {
            return std::make_unique<ConstExpressionAST>(
                MakeStringValue(Token(token).Text())
            );
        }
        if (Match(TokenType::kTrue)) {
            return std::make_unique<ConstExpressionAST>(
                MakeBoolValue(true)
            );
        }
        if (Match(TokenType::kFalse)) {
            return std::make_unique<ConstExpressionAST>(
                MakeBoolValue(false)
            );
        }
        if (Match(TokenType::kNullType)) {
            return std::make_unique<ConstExpressionAST>(
                MakeNullValue()
            );
        }
        if (Match(TokenType::kIdentifier)) {
            return std::make_unique<VariableExpression>(
                token.Text()
            );
        }
        if (Match(TokenType::kLParenthesis)) {
            auto expr = ParseExpression();
            Check(TokenType::kRParenthesis);
            return expr;
        }
        if (Match(TokenType::kFunction)) {
            return ParseFunctionDeclaration();
        }
        throw std::runtime_error("expected literal or number");
    }

    expression ParseFunctionCall(expression function) {
        std::vector<expression> arguments;
        while (Peek().Type() != TokenType::kRParenthesis) {
            arguments.push_back(ParseExpression());
            if (!Match(TokenType::kComma)) {
                break;
            }
        }
        Check(TokenType::kRParenthesis);
        return std::make_unique<FunctionCallExpression>(std::move(function), std::move(arguments));
    }

    expression ParseFunctionDeclaration() {
        Check(TokenType::kLParenthesis);
        std::vector<std::string> arguments;
        while (Peek().Type() == TokenType::kIdentifier) {
            arguments.push_back(Peek().Text());
            Check(TokenType::kIdentifier);
            if (!Match(TokenType::kComma)) {
                break;
            }
        }
        Check(TokenType::kRParenthesis);
        statement body = ParseScopeStatement({TokenType::kEnd}, true);
        Check(TokenType::kEnd);
        Check(TokenType::kFunction);
        return std::make_unique<ConstExpressionAST>(MakeFunctionValue(std::move(body), std::move(arguments)));
    }

    [[nodiscard]] bool Match(TokenType type) const {
        return tokens_->Match(type);
    }

    void Check(TokenType type) const {
        tokens_->Check(type);
    }

    [[nodiscard]] const Token& Peek(size_t additional = 0) const {
        return tokens_->Peek(additional);
    }

    std::unique_ptr<TokenSource> tokens_;

    // tables for translation from token type to operations

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
        {TokenType::kMul,       OperationType::kMulOp},
        {TokenType::kDiv,       OperationType::kDivOp},
        {TokenType::kRemainder, OperationType::kRemainderOp},
    };

    static inline operators_table power_table_ = {
        {TokenType::kPower, OperationType::kPowerOp},
    };

    static inline assignment_table assignment_table_ = {
        {TokenType::kAssign,        AssignmentOperationType::kAssign},
        {TokenType::kPlusAssign,    AssignmentOperationType::kPlusAssign},
        {TokenType::kMinusAssign,   AssignmentOperationType::kMinusAssign},
        {TokenType::kMulAssign,     AssignmentOperationType::kMulAssign},
        {TokenType::kDivAssign,     AssignmentOperationType::kDivAssign},
    };
};