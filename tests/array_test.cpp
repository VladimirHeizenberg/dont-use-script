#include <string>
#include <vector>

#include <lib/interpreter.h>
#include <gtest/gtest.h>

std::string kUnreachable = "239";


TEST(ArrayTestSuite, ArrayPrintTest) {
    std::string code = R"(
        arr = [1, 2, "string", nil, ["hello", "bye"]]
        println(arr[1])
        println(arr[-3])
        println(arr)
    )";
    
    std::string expected = "2\nstring\n[1, 2, \"string\", , [\"hello\", \"bye\"]]\n";
    std::stringstream input(code);
    std::stringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


TEST(ArrayTestSuite, ArrayPushTest) {
    std::string code = R"(
        arr = [1, 2, 3, 4]
        push(arr, 10)
        println(arr)
        push(arr, "hello")
        println(arr)
    )";
    
    std::string expected = "[1, 2, 3, 4, 10]\n[1, 2, 3, 4, 10, \"hello\"]\n";
    std::stringstream input(code);
    std::stringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(ArrayTestSuite, ArrayPopTest) {
    std::string code = R"(
        arr = [1, 2, 3, 4, "hello"]
        pop(arr)
        println(arr)
        pop(arr)
        println(arr)
    )";
    
    std::string expected = "[1, 2, 3, 4]\n[1, 2, 3]\n";
    std::stringstream input(code);
    std::stringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


TEST(ArrayTestSuite, ArraySortTest) {
    std::string code = R"(
        arr = [566, -239, 100500, 2025]
        sort(arr)
        println(arr)
        arr2 = ["aaa", "a", "bb"]
        sort(arr2)
        println(arr2)
    )";
    
    std::string expected = "[-239, 566, 2025, 100500]\n[\"a\", \"aaa\", \"bb\"]\n";
    std::stringstream input(code);
    std::stringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(ArrayTestSuite, InvalidSortTest) {
    std::string code = R"(
        arr = [566, -239, 100500, 2025]
        sort(arr)
        println(arr)
        arr2 = ["aaa", "a", 539, "bb"]
        sort(arr2)
        println(arr2)
    )";
    
    std::stringstream input(code);
    std::stringstream output;
    ASSERT_FALSE(interpret(input, output));
}

TEST(ArrayTestSuite, InsertTest) {
    std::string code = R"(
        arr = [1, 2, "hello", nil, [1, 2]]
        insert(arr, 2, "str")
        println(arr)
    )";
    
    std::stringstream input(code);
    std::stringstream output;

    std::string expected = "[1, 2, \"str\", \"hello\", , [1, 2]]\n";
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(ArrayTestSuite, RemoveTest) {
    std::string code = R"(
        arr = [1, 2, "hello", nil, [1, 2]]
        remove(arr, 2)
        println(arr)
    )";
    
    std::stringstream input(code);
    std::stringstream output;

    std::string expected = "[1, 2, , [1, 2]]\n";
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}
