#include <lib/interpreter.h>
#include <gtest/gtest.h>


TEST(TypesTestSuite, IntTest) {
    std::string code = R"(
        x = 1
        y = 2
        z = 3 * x + y
        print(z)
    )";

    std::string expected = "5";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(TypesTestSuite, BoolTest) {
    std::string code = R"(
        x = 1
        y = 2
        z = 3 * x + y + true * 5
        print(z)
    )";

    std::string expected = "10";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(TypesTestSuite, BoolTest2) {
    std::string code = R"(
        x = 1
        y = 2
        z = 10 * (x < y) + (x < y) + 239 * (2 != 2)
        print(z)
    )";

    std::string expected = "11";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(TypesTestSuite, StringTest) {
    std::string code = R"(
        x = 1
        y = "aba"
        z = 3 * x * y * true + "caba"
        print(z)
    )";

    std::string expected = "abaabaabacaba";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(TypesTestSuite, StringTest2) {
    std::string code = R"(
        x = 1
        y = "aba"
        z = 3 * x * y + "caba"
        print(z * false)
    )";

    std::string expected;

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(TypesTestSuite, StringTest3) {
    std::string code = R"(
        x = 1
        y = "aba"
        z = 3 * x * y + "caba"
        print(z * true)
    )";

    std::string expected = "abaabaabacaba";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(TypesTestSuite, StringTest4) {
    std::string code = R"(
        x = 1
        y = "aba"
        z = 3 * x * y + "caba"
        print(z * 2)
    )";

    std::string expected = "abaabaabacabaabaabaabacaba";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}



