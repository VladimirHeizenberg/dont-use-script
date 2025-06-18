#include "interpreter.h"

#include "../include/parser/Lexer.h"
#include "../include/parser/Parser.h"
#include "../include/executor/Executor.h"

bool interpret(std::istream& input, std::ostream& output) {
    Lexer lexer(input);
    auto res = lexer.tokenize();
    std::unique_ptr<TokenSource> tokens = std::make_unique<VectorReferenceTokenSource>(res);
    Parser parser(std::move(tokens));
    std::unique_ptr<StatementSource> statement_source = std::make_unique<VectorStatementSource>(
        parser.parse()
    );
    Executor executor(std::move(statement_source), input, output);
    executor.Execute();
    return true;
}