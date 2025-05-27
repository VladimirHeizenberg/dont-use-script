#include "interpreter.h"

#include "parser/Lexer.h"
#include "parser/Parser.h"

bool interpret(std::istream& input, std::ostream& output) {
    Lexer lexer(input);
    VariablesTable table;
    auto res = lexer.tokenize();
    // for (auto c : res) {
    //     std::cout << c.text() << " " << c.type() << "\n";
    // }
    Parser parser(res, table, output);
    auto res2 = parser.parse();
    try {
    for (auto& s : res2) s->execute();
    } catch(...) {
        return false;
    }
    return true;
}