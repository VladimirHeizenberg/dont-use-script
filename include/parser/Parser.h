#pragma once

#include <memory>
#include <vector>
#include <set>

#include "src/parser/headers/Token.h"
#include "src/AST/Expression.h"
#include "src/AST/Statement.h"
#include "src/AST/AssignStatement.h"
#include "src/AST/ArithmeticExpression.h"

#include "include/parser/TokenSource.h"

// vector instead of set!

class Parser {
public:
    using expression = std::unique_ptr<ExpressionAST>;
    using statement = std::unique_ptr<StatementAST>;
    using token_and_op = std::pair<TokenType, OperationType>;
    using operators_table = const std::vector<token_and_op>;
    using token_and_assign = std::pair<TokenType, AssignmentOperationType>;
    using assignment_table = const std::vector<token_and_assign>;

    explicit Parser(std::unique_ptr<TokenSource> tokens);

    std::vector<statement> parse();

private:
    statement ParseStatement(bool parsing_function_flag);
    statement ParsePrintStatement();
    statement ParsePrintlnStatement();
    statement ParseIfStatement(bool flag, bool parsing_function_flag);
    statement ParseWhileStatement(bool parsing_function_flag);
    statement ParseForStatement(bool parsing_function_flag);
    statement ParseScopeStatement(const std::set<TokenType>& stop_words, bool parsing_function_flag);
    statement ParseAssignStatement();
    statement ParseReturnStatement();

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

    expression ParseExpression();
    expression ParseLogicalOr();
    expression ParseLogicalAnd();
    expression ParseEqual();
    expression ParseRelation();
    expression ParseAdd();
    expression ParseMult();
    expression ParseUnaryPlusMinus();
    expression ParseLogicalNot();
    expression ParseSuffixExpression();
    expression ParseLiteral();
    expression ParseArray();
    expression ParseFunctionCall(expression function);
    expression ParseFunctionDeclaration();

    [[nodiscard]] bool Match(TokenType type) const;
    void Check(TokenType type) const;
    [[nodiscard]] const Token& Peek(size_t additional = 0) const;

    std::unique_ptr<TokenSource> tokens_;

    static inline const std::set<std::string> reserved_names_for_functions = {
        // functions for nums
        "abs", "ceil", "floor", "round", "sqrt",
        "rnd", "parse_num", "to_string",
        // len
        "len",
        // functions for strings
        "upper", "lower", "split",
        "join", "replace",
        // functions for lists
        "range", "push", "pop", "insert",
        "remove", "sort",
        // system functions
        "print", "println", "read", "stacktrace",
    };

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
