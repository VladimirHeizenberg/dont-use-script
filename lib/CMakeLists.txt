add_library(itmoscript interpreter.cpp
        Value/include/MakeValue.h)

add_subdirectory(Value)

target_link_libraries(itmoscript PUBLIC itmoscript_value_lib)
target_include_directories(itmoscript PUBLIC Value)