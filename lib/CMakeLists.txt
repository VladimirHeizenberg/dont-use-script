add_library(itmoscript interpreter.cpp
        Value/include/MakeValue.h
        AST/BreakStatement.h
        AST/ContinueStatement.h
        AST/ReturnStatement.h
        AST/FunctionCallExpression.h
        AST/ExpressionStatement.h
        AST/IndexExpression.h
        AST/AssignableExpression.h
        AST/ForStatement.h)

add_subdirectory(Value)

target_link_libraries(itmoscript PUBLIC itmoscript_value_lib)
target_include_directories(itmoscript PUBLIC Value)