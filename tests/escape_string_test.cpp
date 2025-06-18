#include <lib/interpreter.h>
#include <gtest/gtest.h>

TEST(EscapeTestSuite, EscapeSequenceQuotesTest) {
    std::string code = R"(
        print("hello \"world\"")
    )";

    std::string expected = R"(hello "world")";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(EscapeTestSuite, EscapeSequenceSlashTest) {
    std::string code = R"(
        print("hello \\world\\")
    )";

    std::string expected = R"(hello \world\)";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(EscapeTestSuite, WrongEscapeSequenceTest) {
    std::string code = R"(
        print("hello "world" ")
    )";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_FALSE(interpret(input, output));
}