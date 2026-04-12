#include "Parser.h"

#include <stdexcept>
#include <functional>
#include <tuple>

#include "ast/ast.h"

#include "value/Value.h"
#include "value/MakeValue.h"

#include "errors/ParserErrors.h"

namespace itmo_script::parser {

// -------------------tables-----------------------

static const std::unordered_map<TokenType, std::function<Parser::expression(const Token&)>> kIdentifierTable = {
    {
        TokenType::kNumber,
        [](const Token& token) {
            return std::make_unique<ast::CnstExpr>(
                value::MakeDouble(std::stod(token.GetText()))
            );
        }
    },
    {
        TokenType::kString,
        [](const Token& token) {
            return std::make_unique<ast::CnstExpr>(
                value::MakeString(Token(token).GetText())
            );
        }
    },
    {
        TokenType::kTrue,
        [](const Token& token) {
            return std::make_unique<ast::CnstExpr>(
                value::MakeBool(true)
            );
        }
    },
    {
        TokenType::kFalse,
        [](const Token& token) {
            return std::make_unique<ast::CnstExpr>(
                value::MakeBool(false)
            );
        }
    },
    {
        TokenType::kNullType,
        [](const Token& token) {
            return std::make_unique<ast::CnstExpr>(
                value::MakeNull()
            );
        }
    },
    {
        TokenType::kIdentifier,
        [](const Token& token) {
            return std::make_unique<ast::LiteralExpr>(
                token.GetText()
            );
        }
    },
};

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

static const Parser::operators_table logical_or_table_ = {
     {TokenType::kLogicalOr, ast::OperationType::kLogicalOr},
};

static const Parser::operators_table logical_and_table_ = {
     {TokenType::kLogicalAnd, ast::OperationType::kLogicalAnd},
};

static const Parser::operators_table equality_table_ = {
    {TokenType::kEqual,     ast::OperationType::kEqual},
    {TokenType::kNotEqual,  ast::OperationType::kNotEqual},
};

static const Parser::operators_table relations_table_ = {
    {TokenType::kLess,            ast::OperationType::kLess},
    {TokenType::kGreater,         ast::OperationType::kGreater},
    {TokenType::kLessOrEqual,     ast::OperationType::kLessOrEqual},
    {TokenType::kGreaterOrEqual,  ast::OperationType::kGreaterOrEqual},
};

static const Parser::operators_table add_table_ = {
    {TokenType::kPlus,   ast::OperationType::kPlusOp},
    {TokenType::kMinus,  ast::OperationType::kMinusOp},
};

static const Parser::operators_table mult_table_ = {
    {TokenType::kAsteriks,       ast::OperationType::kMulOp},
    {TokenType::kSlash,          ast::OperationType::kDivOp},
    {TokenType::kRemainder,      ast::OperationType::kRemainderOp},
};

static const Parser::operators_table power_table_ = {
    {TokenType::kPower, ast::OperationType::kPowerOp},
};

static const Parser::assignment_table assignment_table_ = {
    {TokenType::kAssign,        ast::AssignType::kAssign},
    {TokenType::kPlusAssign,    ast::AssignType::kPlusAssign},
    {TokenType::kMinusAssign,   ast::AssignType::kMinusAssign},
    {TokenType::kMulAssign,     ast::AssignType::kMulAssign},
    {TokenType::kDivAssign,     ast::AssignType::kDivAssign},
};

// -----------------parser-------------------

Parser::Parser(std::unique_ptr<TokenSource> tokens)
: tokens_(std::move(tokens)) {}


std::vector<Parser::statement> Parser::parse() {
    std::vector<statement> result;
    while (!Match(TokenType::kEOF)) {
        result.push_back(ParseStatement(false));
    }
    return result;
}

Parser::statement Parser::ParseStatement(bool parsing_function_flag) {
    if (Match(TokenType::kIf)) return ParseIfStatement(true, parsing_function_flag);
    if (Match(TokenType::kWhile)) return ParseWhileStatement(parsing_function_flag);
    if (Match(TokenType::kFor)) return ParseForStatement(parsing_function_flag);
    if (Match(TokenType::kReturn)) {
        if (!parsing_function_flag) {
            throw std::runtime_error("return outside function");
        }
        return ParseReturnStatement();
    }
    if (Match(TokenType::kBreak)) return std::make_unique<ast::BreakStmt>();
    if (Match(TokenType::kContinue)) return std::make_unique<ast::ContinueStmt>();
    if (Peek().GetType() == TokenType::kIdentifier && Peek(1).GetType() == TokenType::kAssign) {
        return ParseAssignStatement();
    }
    return std::make_unique<ast::ExprStmt>(ParseExpression());
}

Parser::statement Parser::ParseIfStatement(bool flag, bool parsing_function_flag) {
    expression if_expression = ParseExpression();
    Check(TokenType::kThen);
    statement statement_true = ParseScopeStatement(
        {TokenType::kEnd, TokenType::kElif, TokenType::kElse},
        parsing_function_flag
    );
    statement statement_false = std::make_unique<ast::EmptyStmt>();
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
    return std::make_unique<ast::IfStmt>(
        std::move(if_expression), std::move(statement_true), std::move(statement_false)
    );
}

Parser::statement Parser::ParseWhileStatement(bool parsing_function_flag) {
    expression while_expression = ParseExpression();
    statement while_statement = ParseScopeStatement({TokenType::kEnd}, parsing_function_flag);
    Check(TokenType::kEnd);
    Check(TokenType::kWhile);
    return std::make_unique<ast::WhileStmt>(
        std::move(while_expression), std::move(while_statement)
    );
}

Parser::statement Parser::ParseForStatement(bool parsing_function_flag) {
    std::string name = Peek().GetText();
    Check(TokenType::kIdentifier);
    Check(TokenType::kIn);
    auto expr = ParseExpression();
    auto scope = ParseScopeStatement({TokenType::kEnd}, parsing_function_flag);
    Check(TokenType::kEnd);
    Check(TokenType::kFor);
    return std::make_unique<ast::ForStmt>(name, std::move(expr), std::move(scope));
}

Parser::statement Parser::ParseScopeStatement(const std::set<TokenType>& stop_words,
                                              bool parsing_function_flag) {
    std::unique_ptr<ast::ScopeStmt> scope = std::make_unique<ast::ScopeStmt>();
    while (!stop_words.contains(Peek().GetType())) {
        scope->add(ParseStatement(parsing_function_flag));
    }
    return scope;
}

Parser::statement Parser::ParseAssignStatement() {
    Token current = Peek();
    if (reserved_names_for_functions.contains(current.GetText())) {
        throw std::runtime_error("It's forbidden to assign system function names");
    }
    for (auto& [token, assignment]: assignment_table_) {
        if (Peek().GetType() == TokenType::kIdentifier && Peek(1).GetType() == token) {
            Check(TokenType::kIdentifier);
            Check(token);
            return std::make_unique<ast::AssignStmt>(
                current.GetText(), ParseExpression(), assignment
            );
        }
    }
    throw std::runtime_error("Invalid assignment syntax");
}

Parser::statement Parser::ParseReturnStatement() {
    return std::make_unique<ast::ReturnStmt>(ParseExpression());
}

Parser::expression Parser::ParseExpression() {
    return ParseLogicalOr();
}

Parser::expression Parser::ParseBinaryOperations(expression (Parser::*next_level)(), const operators_table& table) {
    expression expr = (this->*next_level)();
    while (true) {
        bool matched = false;
        for (const auto& [token, operation] : table) {
            if (Match(token)) {
                expr = std::make_unique<ast::BinExpr>(
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

Parser::expression Parser::ParseLogicalOr() {
    return ParseBinaryOperations(&Parser::ParseLogicalAnd, logical_or_table_);
}

Parser::expression Parser::ParseLogicalAnd() {
    return ParseBinaryOperations(&Parser::ParseEqual, logical_and_table_);
}

Parser::expression Parser::ParseEqual() {
    return ParseBinaryOperations(&Parser::ParseRelation, equality_table_);
}

Parser::expression Parser::ParseRelation() {
    return ParseBinaryOperations(&Parser::ParseAdd, relations_table_);
}

Parser::expression Parser::ParseAdd() {
    return ParseBinaryOperations(&Parser::ParseMult, add_table_);
}

Parser::expression Parser::ParseMult() {
    return ParseBinaryOperations(&Parser::ParseUnaryPlusMinus, mult_table_);
}

Parser::expression Parser::ParseUnaryPlusMinus() {
    if (Match(TokenType::kMinus)) {
        return std::make_unique<ast::UnaryExpr>(
            ast::OperationType::kMinusOp, ParseUnaryPlusMinus()
        );
    }
    if (Match(TokenType::kPlus)) {
        return std::make_unique<ast::UnaryExpr>(
            ast::OperationType::kPlusOp, ParseUnaryPlusMinus()
        );
    }
    return ParsePower();
}

Parser::expression Parser::ParsePower() {
    expression expr = ParseLogicalNot();
    if (Match(TokenType::kPower)) {
        expr = std::make_unique<ast::BinExpr>(
                ast::OperationType::kPowerOp, std::move(expr), ParsePower()
            );
    }
    return expr;
}

Parser::expression Parser::ParseLogicalNot() {
    if (Match(TokenType::kLogicalNot)) {
        return std::make_unique<ast::UnaryExpr>(
            ast::OperationType::kLogicalNot, ParseSuffixExpression()
        );
    }
    return ParseSuffixExpression();
}

Parser::expression Parser::ParseSuffixExpression() {
    auto expr = ParseLiteral();

    while (true) {
        if (Match(TokenType::kLParenthesis)) {
            expr = ParseFunctionCall(std::move(expr));
        } else if (Match(TokenType::kLBracket)) {
            auto index_expr = ParseExpression();
            Check(TokenType::kRBracket);
            expr = std::make_unique<ast::IndexExpr>(std::move(expr), std::move(index_expr));
        } else break;
    }
    return expr;
}

Parser::expression Parser::ParseLiteral() {
    auto token = Peek();
    if (kIdentifierTable.contains(token.GetType())) {
        Check(token.GetType());
        return kIdentifierTable.at(token.GetType())(token);
    }
    if (Match(TokenType::kLParenthesis)) {
        auto expr = ParseExpression();
        Check(TokenType::kRParenthesis);
        return expr;
    }
    if (Match(TokenType::kFunction)) {
        return ParseFunctionDeclaration();
    }
    if (Match(TokenType::kLBracket)) {
        return ParseArray();
    }
    throw std::runtime_error("expected literal or number");
}

Parser::expression Parser::ParseArray() {
    std::vector<expression> exprs;
    while (!Match(TokenType::kRBracket)) {
        exprs.push_back(ParseExpression());
        if (!Match(TokenType::kComma)) {
            Check(TokenType::kRBracket);
            break;
        }
    }
    return std::make_unique<ast::ArrayExpr>(std::move(exprs));
}

Parser::expression Parser::ParseFunctionCall(expression function) {
    std::vector<expression> arguments;
    while (Peek().GetType() != TokenType::kRParenthesis) {
        arguments.push_back(ParseExpression());
        if (!Match(TokenType::kComma)) {
            break;
        }
    }
    Check(TokenType::kRParenthesis);
    return std::make_unique<ast::FunctionCallExpr>(std::move(function), std::move(arguments));
}

Parser::expression Parser::ParseFunctionDeclaration() {
    Check(TokenType::kLParenthesis);
    std::vector<std::string> arguments;
    while (Peek().GetType() == TokenType::kIdentifier) {
        arguments.push_back(Peek().GetText());
        Check(TokenType::kIdentifier);
        if (!Match(TokenType::kComma)) {
            break;
        }
    }
    Check(TokenType::kRParenthesis);
    statement body = ParseScopeStatement({TokenType::kEnd}, true);
    Check(TokenType::kEnd);
    Check(TokenType::kFunction);
    return std::make_unique<ast::CnstExpr>(value::MakeFunction(std::move(body), std::move(arguments)));
}

bool Parser::Match(TokenType type) const {
    return tokens_->Match(type);
}

void Parser::Check(TokenType type) const {
    tokens_->Check(type);
}

const Token& Parser::Peek(size_t additional) const {
    return tokens_->Peek(additional);
}


} // namespace itmo_script::parser