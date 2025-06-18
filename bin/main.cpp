#include <iostream>
#include <sstream>

#include "include/parser/Lexer.h"
#include "include/parser/Parser.h"
#include "include/executor/Executor.h"

int main(int argc, char** argv) {
    std::string code = R"(
        println([1, 2] + [3, "hello \"world\""])
    )";
    std::stringstream ss(code);
    Lexer lexer(ss);
    auto res = lexer.tokenize();
    std::unique_ptr<TokenSource> tokens = std::make_unique<VectorReferenceTokenSource>(res);
    Parser parser(std::move(tokens));
    std::unique_ptr<StatementSource> statement_source = std::make_unique<VectorStatementSource>(
        parser.parse()
    );
    Executor executor(std::move(statement_source), ss, std::cout);
    executor.Execute();
}