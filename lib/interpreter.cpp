#include "interpreter.h"

#include "src/parser/Lexer.h"
#include "src/parser/Parser.h"
#include "src/executor/Executor.h"

bool interpret(std::istream& input, std::ostream& output) {
    itmo_script::parser::Lexer lexer(input);
    try {
        auto res = lexer.Tokenize();
        std::unique_ptr<itmo_script::parser::TokenSource> tokens = (
            std::make_unique<itmo_script::parser::VectorReferenceTokenSource>(res)
        );
        itmo_script::parser::Parser parser(std::move(tokens));
        std::unique_ptr<itmo_script::executor::StatementSource> statement_source = (
            std::make_unique<itmo_script::executor::VectorStatementSource>(parser.parse())
        );
        itmo_script::executor::Executor executor(std::move(statement_source), input, output);
        executor.Execute();
    } catch (std::runtime_error& e) {
        std::cerr << e.what();
        return false;
    }
    return true;
}