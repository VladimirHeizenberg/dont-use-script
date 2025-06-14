#include "interpreter.h"

#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "executor/Executor.h"

bool interpret(std::istream& input, std::ostream& output) {
    std::unique_ptr<CharSource> source = std::make_unique<StreamCharSource>(input);
    Lexer lexer(std::move(source));
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