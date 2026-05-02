#include "interpreter.h"

#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "executor/Executor.h"

bool interpret(std::istream& input, std::ostream& output) {
    dont_use_script::parser::Lexer lexer(input);
    try {
        auto res = lexer.Tokenize();
        std::unique_ptr<dont_use_script::parser::TokenSource> tokens = (
            std::make_unique<dont_use_script::parser::VectorReferenceTokenSource>(res)
        );
        dont_use_script::parser::Parser parser(std::move(tokens));
        std::unique_ptr<dont_use_script::executor::StatementSource> statement_source = (
            std::make_unique<dont_use_script::executor::VectorStatementSource>(parser.parse())
        );
        dont_use_script::executor::Executor executor(std::move(statement_source), input, output);
        executor.Execute();
    } catch (std::runtime_error& e) {
        std::cerr << e.what();
        return false;
    }
    return true;
}