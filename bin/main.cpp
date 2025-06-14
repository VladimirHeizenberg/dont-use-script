#include <iostream>
#include <sstream>

#include "../lib/parser/Lexer.h"
#include "../lib/parser/Parser.h"
#include "lib/executor/Executor.h"

int main(int argc, char** argv) {
    std::string code = R"(
        foo = function(a, b)
            return a + b
        end function

        c = foo(4 + (2 * 3), 4 - (2 * 3)) * 5 + 10
        println(c)

        foo(1, 2)
    )";
    std::stringstream ss(code);
    std::unique_ptr<CharSource> source = std::make_unique<StreamCharSource>(ss);
    Lexer lexer(std::move(source));
    auto res = lexer.tokenize();
    std::unique_ptr<TokenSource> tokens = std::make_unique<VectorReferenceTokenSource>(res);
    Parser parser(std::move(tokens));
    std::unique_ptr<StatementSource> statement_source = std::make_unique<VectorStatementSource>(
        parser.parse()
    );
    Executor executor(std::move(statement_source), ss, std::cout);
    executor.Execute();
}