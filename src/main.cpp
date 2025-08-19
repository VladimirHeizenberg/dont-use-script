#include <iostream>
#include <sstream>
#include <fstream>

#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "executor/Executor.h"
#include <ArgParser.h>

int main(int argc, char** argv) {
    ArgumentParser::ArgParser cmd_parser("ITMOScript cmd parser");
    cmd_parser.AddStringArgument('i', "input", "Source code file");
    cmd_parser.Parse(argc, argv);

    std::string code = R"(
        a = 1
        b = 2
        c = 123
        d = (a + b *c) -c
        println(a)
        println(b)
        println(c)
        println(d)
    )";

    std::stringstream source_code(code);
    itmo_script::parser::Lexer lexer(source_code);
    try {
        auto result = lexer.Tokenize();
        std::unique_ptr<itmo_script::parser::TokenSource> tokens = 
            std::make_unique<itmo_script::parser::VectorReferenceTokenSource>(result);
        itmo_script::parser::Parser parser(std::move(tokens));

        std::unique_ptr<itmo_script::executor::StatementSource> statement_source = std::make_unique<itmo_script::executor::VectorStatementSource>(
            parser.parse()
        );
        itmo_script::executor::Executor executor(std::move(statement_source), std::cin, std::cout);
        executor.Execute();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
    // for (auto& c : result) {
    //     std::cout << Parser::token_to_string(c.GetType()) << " ";
    // }
}