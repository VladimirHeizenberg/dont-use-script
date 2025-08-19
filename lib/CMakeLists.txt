include(FetchContent)

FetchContent_Declare(
    argparser
    GIT_REPOSITORY https://github.com/VladimirHeizenberg/cmd-arguments-parser
    GIT_TAG v.1.0
)

FetchContent_MakeAvailable(argparser)

target_include_directories(argparser INTERFACE
    ${argparser_SOURCE_DIR}/lib
)


add_library(itmoscript interpreter.cpp)

target_include_directories(itmoscript PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(itmoscript
        PUBLIC parser
        value
)