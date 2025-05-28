#include <iostream>
#include <sstream>

#include "lib/parser/Lexer.h"
#include "lib/parser/Parser.h"

int main(int argc, char** argv) {
    std::string code = R"(
        a = 1 and 2 or 3
        println(a)
        if 2 + 2 == 4 and 3 * 5 == 15 then 
            println(5) 
        end if
        b = true
        c = false
        if b or c then
            println(566)
        end if

        if b and c then
            println(239)
        else
            println(566)
        end if
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