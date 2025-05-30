#include <iostream>
#include <sstream>

#include "lib/parser/Lexer.h"
#include "lib/parser/Parser.h"

int main(int argc, char** argv) {
    int a = 5;
    std::string code = R"(
        a = 2
        while a < 10
            println(a)
            a += 1
            if a == 5 then
                println(239)
            end if
        end while
    )";
    std::stringstream ss(code);
    Lexer lexer(ss);
    VariablesTable table;
    auto res = lexer.tokenize();
    // for (auto c : res) {
    //     std::cout << c.text() << " " << c.type() << "\n";
    // }
    Parser parser(res, table, std::cout);
    auto res2 = parser.parse();
    for (auto& s : res2) s->execute();
    // std::cout << table.Get("c") << "\n";
    // std::cout << table.Get("d") << "\n";
    // // std::cout << res2.size() << "\n";
    // std::cout << res2[0]->evaluate();
    // return 0;


}