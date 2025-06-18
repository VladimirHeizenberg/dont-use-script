add_library(itmoscript
        interpreter.cpp
)

target_include_directories(itmoscript PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(itmoscript
        PUBLIC itmoscript_parser
        itmoscript_value_lib
)
