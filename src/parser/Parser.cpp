#include "include/parser/Parser.h"

#include "src/AST/AST.h"
#include "src/value/headers/Value.h"

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
    if (Match(TokenType::kPrint)) return ParsePrintStatement();
    if (Match(TokenType::kPrintln)) return ParsePrintlnStatement();
    if (Match(TokenType::kIf)) return ParseIfStatement(true, parsing_function_flag);
    if (Match(TokenType::kWhile)) return ParseWhileStatement(parsing_function_flag);
    if (Match(TokenType::kFor)) return ParseForStatement(parsing_function_flag);
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


Parser::statement Parser::ParsePrintStatement() {
    Check(TokenType::kLParenthesis);
    statement print = std::make_unique<PrintStatement>(
        ParseExpression()
    );
    Check(TokenType::kRParenthesis);
    return print;
}

Parser::statement Parser::ParsePrintlnStatement() {
    Check(TokenType::kLParenthesis);
    statement println = std::make_unique<PrintlnStatement>(
        ParseExpression()
    );
    Check(TokenType::kRParenthesis);
    return println;
}

Parser::statement Parser::ParseIfStatement(bool flag, bool parsing_function_flag) {
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

Parser::statement Parser::ParseWhileStatement(bool parsing_function_flag) {
    expression while_expression = ParseExpression();
    statement while_statement = ParseScopeStatement({TokenType::kEnd}, parsing_function_flag);
    Check(TokenType::kEnd);
    Check(TokenType::kWhile);
    return std::make_unique<WhileStatement>(
        std::move(while_expression), std::move(while_statement)
    );
}

Parser::statement Parser::ParseForStatement(bool parsing_function_flag) {
    std::string name = Peek().Text();
    Check(TokenType::kIdentifier);
    Check(TokenType::kIn);
    auto expr = ParseExpression();
    auto scope = ParseScopeStatement({TokenType::kEnd}, parsing_function_flag);
    Check(TokenType::kEnd);
    Check(TokenType::kFor);
    return std::make_unique<ForStatement>(name, std::move(expr), std::move(scope));
}

Parser::statement Parser::ParseScopeStatement(const std::set<TokenType>& stop_words,
                                              bool parsing_function_flag) {
    std::unique_ptr<ScopeStatement> scope = std::make_unique<ScopeStatement>();
    while (!stop_words.contains(Peek().Type())) {
        scope->add(ParseStatement(parsing_function_flag));
    }
    return scope;
}

Parser::statement Parser::ParseAssignStatement() {
    Token current = Peek();
    if (reserved_names_for_functions.contains(current.Text())) {
        throw std::runtime_error("It's forbidden to assign system function names");
    }
    for (auto& [token, assignment]: assignment_table_) {
        if (Peek().Type() == TokenType::kIdentifier && Peek(1).Type() == token) {
            Check(TokenType::kIdentifier);
            Check(token);
            return std::make_unique<AssignStatement>(
                current.Text(), ParseExpression(), assignment
            );
        }
    }
    throw std::runtime_error("Invalid assignment syntax");
}

Parser::statement Parser::ParseReturnStatement() {
    return std::make_unique<ReturnStatement>(ParseExpression());
}

Parser::expression Parser::ParseExpression() {
    return ParseLogicalOr();
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

Parser::expression Parser::ParseLogicalNot() {
    if (Match(TokenType::kLogicalNot)) {
        return std::make_unique<UnaryExpressionAST>(
            OperationType::kLogicalNot, ParseSuffixExpression()
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
            expr = std::make_unique<IndexExpression>(std::move(expr), std::move(index_expr));
        } else break;
    }
    return expr;
}

Parser::expression Parser::ParseLiteral() {
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
    return std::make_unique<ArrayExpression>(std::move(exprs));
}

Parser::expression Parser::ParseFunctionCall(expression function) {
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

Parser::expression Parser::ParseFunctionDeclaration() {
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

bool Parser::Match(TokenType type) const {
    return tokens_->Match(type);
}
void Parser::Check(TokenType type) const {
    tokens_->Check(type);
}

const Token& Parser::Peek(size_t additional) const {
    return tokens_->Peek(additional);
}
