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

class ArrayArithmeticExpressionsTestSuite : public ::testing::TestWithParam<ExpressionCase> {};


TEST_P(ArrayArithmeticExpressionsTestSuite, EvaluateArraysExpression) {
    std::stringstream ss("print(" + GetParam().code + ")");
    std::stringstream out;
    EXPECT_TRUE(interpret(ss, out));
    EXPECT_EQ(out.str(), GetParam().expected);
}


INSTANTIATE_TEST_SUITE_P(
    ArithmeticExpressionTestCases,
    ArrayArithmeticExpressionsTestSuite,
    testing::Values(
        ExpressionCase{"[1, 2, 3, 4, 5]", "[1, 2, 3, 4, 5]"},
        ExpressionCase{"[1, 2] + [3, 4, 5]", "[1, 2, 3, 4, 5]"},
        ExpressionCase{"[1, 2, 3, 4] * 4", "[1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4]"}
    )
);