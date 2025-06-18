add_library(
        itmoscript_value_lib
        StringValue.cpp
        DoubleValue.cpp
        BoolValue.cpp
        Operations.cpp
        MakeValue.cpp
        FunctionValue.cpp
        NullValue.cpp
        ArrayValue.cpp
)

target_include_directories(itmoscript_value_lib PUBLIC ${PROJECT_SOURCE_DIR})