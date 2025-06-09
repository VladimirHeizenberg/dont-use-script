#include <iostream>
#include <sstream>

#include "lib/parser/Lexer.h"
#include "lib/parser/Parser.h"
#include "lib/executor/Executor.h"

int main(int argc, char** argv) {
    std::string code = R"(
        v = 100 * 2 + 10 * 3 + 9
        if v == 30 then // this is the first comment
            print(30) // this one more
        elif v == 366 then // one more
            print(366)
        elif v == 239 then
            print(239) // hehey
        else
            print(0)
        end if
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