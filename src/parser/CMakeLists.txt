add_library(
        itmoscript_parser
        Lexer.cpp
        Parser.cpp
        TokenSource.cpp
        Token.cpp
        ../AST/OperationType.h
        ../AST/AssignmentOperationType.h
        ../executor/Executor.cpp
)

target_include_directories(itmoscript_parser PUBLIC ${PROJECT_SOURCE_DIR})