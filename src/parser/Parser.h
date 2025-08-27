#pragma once

#include <memory>
#include <vector>
#include <set>

#include "ast/expression/Expression.h"
#include "ast/expression/OperationType.h"
#include "ast/statement/Statement.h"
#include "ast/statement/AssignType.h"
#include "TokenType.h"
#include "TokenSource.h"


namespace itmo_script::parser {

class Parser {
public:
    using expression = std::unique_ptr<ast::Expression>;
    using statement = std::unique_ptr<ast::Statement>;
    using token_and_op = std::pair<TokenType, ast::OperationType>;
    using operators_table = std::vector<token_and_op>;
    using token_and_assign = std::pair<TokenType, ast::AssignType>;
    using assignment_table = std::vector<token_and_assign>;

    explicit Parser(std::unique_ptr<TokenSource> tokens);

    std::vector<statement> parse();

private:
    statement ParseStatement(bool parsing_function_flag);
    statement ParseIfStatement(bool flag, bool parsing_function_flag);
    statement ParseWhileStatement(bool parsing_function_flag);
    statement ParseForStatement(bool parsing_function_flag);
    statement ParseScopeStatement(const std::set<TokenType>& stop_words, bool parsing_function_flag);
    statement ParseAssignStatement();
    statement ParseReturnStatement();

    expression ParseBinaryOperations(expression (Parser::*next_level)(), const operators_table& table);

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
};

} // namespace itmo_script::parser