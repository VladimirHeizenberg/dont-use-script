add_library(
    parser
    Lexer.cpp
    Parser.cpp
    TokenSource.cpp
)

target_link_libraries(parser PUBLIC value)

target_include_directories(parser PUBLIC ${PROJECT_SOURCE_DIR}/src)