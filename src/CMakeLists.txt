add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(parser)
add_subdirectory(value)

target_link_libraries(${PROJECT_NAME} PRIVATE parser argparser)