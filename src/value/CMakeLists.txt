add_library(
    value
    Value.cpp
    MakeValue.cpp
    Operation.cpp
)

target_include_directories(value PUBLIC ${PROJECT_SOURCE_DIR}/src)