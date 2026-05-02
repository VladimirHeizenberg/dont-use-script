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


add_library(dont_use_script interpreter.cpp)

target_include_directories(dont_use_script PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(dont_use_script
        PUBLIC parser
        value
)