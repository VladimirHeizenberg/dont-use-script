#include <iostream>
#include <sstream>

#include "lib/parser/Lexer.h"
#include "lib/parser/Parser.h"

int main(int argc, char** argv) {
    std::string code = R"(
        a = +3
        b = +2.23e+2
        c = (a + b) - 239.2e3 + a * b
        d = c / b
        e = true
        println(d)
        println(c)
        print(e)
    )";
    std::stringstream ss(code);
    Lexer lexer(ss);
    VariablesTable table;
    auto res = lexer.tokenize();
    for (auto c : res) {
        std::cout << c.text() << " " << c.type() << "\n";
    }
    Parser parser(res, table, std::cout);
    auto res2 = parser.parse();
    for (auto& s : res2) s->execute();
    // std::cout << table.Get("c") << "\n";
    // std::cout << table.Get("d") << "\n";
    // // std::cout << res2.size() << "\n";
    // std::cout << res2[0]->evaluate();
    // return 0;
}