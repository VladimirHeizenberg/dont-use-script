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

class IndexExpressionTestSuite : public ::testing::TestWithParam<ExpressionCase> {};


TEST_P(IndexExpressionTestSuite, IndexExpressionTest) {
    std::stringstream ss(GetParam().code);
    std::stringstream out;
    EXPECT_TRUE(interpret(ss, out));
    EXPECT_EQ(out.str(), GetParam().expected);
}


INSTANTIATE_TEST_SUITE_P(
    IndexExpressionTestCases,
    IndexExpressionTestSuite,
    testing::Values(

        // arrays
        ExpressionCase{
            "mas = [1, 2, 3, 4, 5] print(mas[0])",
            "1"
        },
        ExpressionCase{
            "mas = [1, 2, 3, 4, 5] print(mas[4])",
            "5"
        },
        ExpressionCase{
            "mas = [1, 2, 3, 4, 5] print(mas[-1])",
            "5"
        },
        ExpressionCase{
            "mas = [1, 2, 3, 4, 5] print(mas[-3])",
            "3"
        },

        // strings
        ExpressionCase{
            "str = \"abcdef\" print(str[0])",
            "a"
        },
        ExpressionCase{
            "str = \"abcdef\" print(str[5])",
            "f"
        },
        ExpressionCase{
            "str = \"abcdef\" print(str[-1])",
            "f"
        },
        ExpressionCase{
            "str = \"abcdef\" print(str[-3])",
            "d"
        },

        // rvalue index
        ExpressionCase{
            "print([1, 2, 3, 4, 5][0])",
            "1"
        },
        ExpressionCase{
            "print([1, 2, 3, 4, 5][3])",
            "4"
        },
        ExpressionCase{
            "print([1, 2, 3, 4, 5][-1])",
            "5"
        },
        ExpressionCase{
            "print([1, 2, 3, 4, 5][-3])",
            "3"
        },
        ExpressionCase{
            "print(\"abcdef\"[0])",
            "a"
        },
        ExpressionCase{
            "print(\"abcdef\"[3])",
            "d"
        },
        ExpressionCase{
            "print(\"abcdef\"[-1])",
            "f"
        },
        ExpressionCase{
            "print(\"abcdef\"[-3])",
            "d"
        }
    )
);