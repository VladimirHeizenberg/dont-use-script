#include "interpreter.h"

#include "parser/Lexer.h"
#include "parser/Parser.h"

bool interpret(std::istream& input, std::ostream& output) {
    // std::unique_ptr<CharSource> source = std::make_unique<StreamCharSource>(input);
    // Lexer lexer(std::move(source));
    // VariablesTable table;
    // auto res = lexer.tokenize();
    // std::unique_ptr<TokenSource> tokens = std::make_unique<VectorReferenceTokenSource>(res);
    // Parser parser(std::move(tokens), table, std::cout);
    // auto res2 = parser.parse();
    // try {
    // for (auto& s : res2) s->execute();
    // } catch(...) {
    //     return false;
    // }
    // return true;
}