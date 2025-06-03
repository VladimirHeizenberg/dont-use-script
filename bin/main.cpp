#include <iostream>
#include <sstream>

#include "lib/parser/Lexer.h"
#include "lib/parser/Parser.h"
#include "lib/executor/Executor.h"

int main(int argc, char** argv) {
    std::string code = R"(
        a = 2
        while a < 10
            println(a)
            if a == 5 then
                println(239)
            end if
            a += 1
        end while
    )";
    std::stringstream ss(code);
    std::unique_ptr<CharSource> source = std::make_unique<StreamCharSource>(ss);
    Lexer lexer(std::move(source));
    VariablesTable table;
    auto res = lexer.tokenize();
    std::unique_ptr<TokenSource> tokens = std::make_unique<VectorReferenceTokenSource>(res);
    Parser parser(std::move(tokens));
    std::unique_ptr<StatementSource> statement_source = std::make_unique<VectorStatementSource>(
        parser.parse()
    );
    Executor executor(std::move(statement_source), ss, std::cout);
    executor.Execute();
    try {
    } catch(std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }
}