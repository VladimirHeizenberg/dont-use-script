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
    if (!cmd_parser.Parse(argc, argv)) {
        std::cout << cmd_parser.HelpDescription();
        return 1;
    }

    std::fstream source_code(cmd_parser.GetValue<std::string>("input"));
    dont_use_script::parser::Lexer lexer(source_code);
    try {
        auto result = lexer.Tokenize();
        std::unique_ptr<dont_use_script::parser::TokenSource> tokens = 
            std::make_unique<dont_use_script::parser::VectorReferenceTokenSource>(result);
        dont_use_script::parser::Parser parser(std::move(tokens));

        std::unique_ptr<dont_use_script::executor::StatementSource> statement_source = std::make_unique<dont_use_script::executor::VectorStatementSource>(
            parser.parse()
        );
        dont_use_script::executor::Executor executor(std::move(statement_source), std::cin, std::cout);
        executor.Execute();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
    // for (auto& c : result) {
    //     std::cout << Parser::token_to_string(c.GetType()) << " ";
    // }
}