#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include <lib/interpreter.h>


struct ExpressionCase {
    std::string code;
    std::string expected;
};

inline std::ostream& operator<<(std::ostream& out, const ExpressionCase& test_case) {
    return out << test_case.code;
}

class StringArithmeticExpressionsTestSuite : public ::testing::TestWithParam<ExpressionCase> {};


TEST_P(StringArithmeticExpressionsTestSuite, EvaluateStringsExpression) {
    std::stringstream ss("print(" + GetParam().code + ")");
    std::stringstream out;
    EXPECT_TRUE(interpret(ss, out));
    EXPECT_EQ(out.str(), GetParam().expected);
}


INSTANTIATE_TEST_SUITE_P(
    ArithmeticExpressionTestCases,
    StringArithmeticExpressionsTestSuite,
    testing::Values(
        // Literals and base arithmetics
        ExpressionCase{"\"hello\"", "hello"},
        ExpressionCase{"\"hello\" + \"world\"", "helloworld"},
        ExpressionCase{
            R"("hello" * 4)",
            "hellohellohellohello"
        },
        ExpressionCase{
            R"("helloworld" - "world")",
            "hello"
        },
        ExpressionCase{
             R"("helloworld" - "hello")",
            "helloworld"
        },

        // escape seq
        ExpressionCase{
            R"("hello\nworld")",
            "hello\nworld"
        },
        ExpressionCase{
            R"("hello\"world\"")",
            "hello\"world\""
        },
        ExpressionCase{
            R"("hello\tworld")",
            "hello\tworld"
        },
        ExpressionCase{
            R"("hello\rworld")",
            "hello\rworld"
        },
        ExpressionCase{
            R"("hello\'world\'")",
            R"(hello'world')"
        }
    )
);