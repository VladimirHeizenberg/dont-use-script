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

class NumberArithmeticExpressionsTestSuite : public ::testing::TestWithParam<ExpressionCase> {};


TEST_P(NumberArithmeticExpressionsTestSuite, EvaluateExpression) {
    std::stringstream ss("print(" + GetParam().code + ")");
    std::stringstream out;
    EXPECT_TRUE(interpret(ss, out));
    EXPECT_EQ(out.str(), GetParam().expected);
}


INSTANTIATE_TEST_SUITE_P(
    ArithmeticExpressionTestCases,
    NumberArithmeticExpressionsTestSuite,
    testing::Values(
        // Literals and base arithmetics
        ExpressionCase{"1", "1"},
        ExpressionCase{"-5", "-5"},
        ExpressionCase{"+10", "10"},
        ExpressionCase{"1 + 2", "3"},
        ExpressionCase{"7 - 4", "3"},
        ExpressionCase{"3 * 4", "12"},
        ExpressionCase{"8 / 2", "4"},
        ExpressionCase{"9 % 4", "1"},
        ExpressionCase{"2 ^ 3", "8"},

        // Priority
        ExpressionCase{"2 + 3 * 4", "14"},
        ExpressionCase{"(2 + 3) * 4", "20"},
        ExpressionCase{"2 * 3 ^ 2", "18"},
        ExpressionCase{"(2 * 3) ^ 2", "36"},
        ExpressionCase{"2 + 3 * 4 ^ 2", "50"},
        ExpressionCase{"2 ^ 3 ^ 2", "512"}, // right-associative
        ExpressionCase{"((1 + 2) * (3 + 4))", "21"},
        ExpressionCase{"(1 + (2 * (3 + 4)))", "15"},
        ExpressionCase{"((1 + 2) * (3 + 4)) ^ 2", "441"},
        ExpressionCase{"-2^3", "-8"},
        ExpressionCase{"-2^4", "-16"},
        ExpressionCase{"(-2)^4", "16"},
        ExpressionCase{"(2 + 3) * (4 + 1)", "25"},
        ExpressionCase{"2 * (3 + 4 * (5 + 6))", "94"},
        ExpressionCase{"(2 + 3 * (4 + 1)) ^ 2", "289"},
        ExpressionCase{"(2 ^ 3) ^ 2", "64"},
        ExpressionCase{"2 ^ (3 ^ 2)", "512"},
        ExpressionCase{"-(-2 ^ 3)", "8"},
        ExpressionCase{"-(2 ^ (1 + 2))", "-8"},
        ExpressionCase{"(-2) ^ (1 + 2)", "-8"},
        ExpressionCase{"20 / 5 / 2", "2"},
        ExpressionCase{"20 / 2 / 5", "2"},


        // UnaryPlusMinus
        ExpressionCase{"-3 + 5", "2"},
        ExpressionCase{"-(3 + 5)", "-8"},
        ExpressionCase{"- -3", "3"},
        ExpressionCase{"- - -3", "-3"},
        ExpressionCase{"-(-3)", "3"},
        ExpressionCase{"+3", "3"},

        // Comparison
        ExpressionCase{"1 == 1", "true"},
        ExpressionCase{"1 != 2", "true"},
        ExpressionCase{"3 < 4", "true"},
        ExpressionCase{"4 > 3", "true"},
        ExpressionCase{"3 <= 3", "true"},
        ExpressionCase{"4 >= 5", "false"},
        ExpressionCase{"2 + 2 == 4", "true"},
        ExpressionCase{"2 * 2 == 2 + 2", "true"},
        ExpressionCase{"2 * 2 < 3 ^ 2", "true"},
        ExpressionCase{"3 ^ 2 < 2 ^ 3", "false"},

        // boolean logic
        ExpressionCase{"true and false", "false"},
        ExpressionCase{"true or false", "true"},
        ExpressionCase{"not true", "false"},
        ExpressionCase{"not false", "true"},
        ExpressionCase{"1 < 2 and 3 < 4", "true"},
        ExpressionCase{"1 < 2 and 3 > 4", "false"},
        ExpressionCase{"not (1 == 2)", "true"},
        ExpressionCase{"(1 == 1) or (2 == 3)", "true"},
        ExpressionCase{"(1 == 2) or (2 == 3)", "false"},
        ExpressionCase{"(1 == 2) or not (2 == 3)", "true"},
        ExpressionCase{"true and not false", "true"},
        ExpressionCase{"not true and false", "false"},
        ExpressionCase{"(1 < 2) and (2 < 3) and (3 < 4)", "true"},
        ExpressionCase{"(1 < 2) and (2 > 3) or (3 < 4)", "true"},
        ExpressionCase{"not (1 < 2 and 3 > 4)", "true"},
        ExpressionCase{"not (1 < 2) and 3 > 4", "false"},
        ExpressionCase{"(1 + 2) == 3 and (2 * 2) == 4", "true"},
        ExpressionCase{"(1 + 2) == 4 or (2 * 2) == 4", "true"},
        ExpressionCase{"(2 ^ 3) == 8 and not (3 ^ 2 == 8)", "true"},
        ExpressionCase{"(5 / 2) == 2.5", "true"},
        ExpressionCase{"(5 / 2) == 2", "false"},
        ExpressionCase{"(5 % 2) == 1", "true"},
        ExpressionCase{"(2 ^ 3) > (3 ^ 2)", "false"},


        // floating
        ExpressionCase{"1.5 + 2.3", "3.8"},
        ExpressionCase{"2.0 * 0.5", "1"},
        ExpressionCase{"5 / 2", "2.5"},
        ExpressionCase{"5.0 / 2.0", "2.5"},
        ExpressionCase{"1.2e3 + 3", "1203"},
        ExpressionCase{"1.23e-2 * 100", "1.23"},
        ExpressionCase{"-1.5e2", "-150"},
        ExpressionCase{"2.5 ^ 2", "6.25"},
        ExpressionCase{"2 + 3.5", "5.5"},
        ExpressionCase{"3.5 - 1", "2.5"},
        ExpressionCase{"2 * 0.5", "1"},
        ExpressionCase{"4 / 2.0", "2"},
        ExpressionCase{"2.0 ^ 3", "8"}
    )
);
