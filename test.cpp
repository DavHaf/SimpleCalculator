#include <gtest/gtest.h>
#include <stdexcept>

#include "equation.cpp"

TEST(Equation, NoOperation) {
    Equation eq("7");
    long double result = eq.solution();
    EXPECT_EQ(result, 7);
}

TEST(Equation, Addition) {
    Equation eq("2+2");
    long double result = eq.solution();
    EXPECT_EQ(result, 4);
}

TEST(Equation, Subtraction) {
    Equation eq("1 - 3");
    long double result = eq.solution();
    EXPECT_EQ(result, -2);
}

TEST(Equation, Multiplication) {
    Equation eq("3 * -4");
    long double result = eq.solution();
    EXPECT_EQ(result, -12);
}

TEST(Equation, Division) {
    Equation eq("3/2");
    long double result = eq.solution();
    EXPECT_EQ(result, 1.5);
}

TEST(Equation, Exponent) {
    Equation eq("2**-1");
    long double result = eq.solution();
    EXPECT_EQ(result, 0.5);
}

TEST(Equation, DivisionFloor) {
    Equation eq("48//-3");
    long double result = eq.solution();
    EXPECT_EQ(result, -16);
}

TEST(Equation, NegativeStart) {
    Equation eq("-1+5");
    long double result = eq.solution();
    EXPECT_EQ(result, 4);
}

TEST(Equation, Parentheses0) {
    Equation eq("(1+4)");
    long double result = eq.solution();
    EXPECT_EQ(result, 5);
}

TEST(Equation, Parentheses1) {
    Equation eq("6*(1+4)");
    long double result = eq.solution();
    EXPECT_EQ(result, 30);
}

TEST(Equation, Parentheses2) {
    Equation eq("11 * (1+4) // (4*(2+4))");
    long double result = eq.solution();
    EXPECT_EQ(result, 2);
}

TEST(Equation, Parentheses3) {
    Equation eq("1-(2+(3-(4+5)))");
    long double result = eq.solution();
    EXPECT_EQ(result, 5);
}

TEST(Equation, Parentheses4) {
    Equation eq("(((1-2)+3)-4)+5");
    long double result = eq.solution();
    EXPECT_EQ(result, 3);
}

TEST(Equation, OrderOfOperations1) {
    Equation eq("3+5-16/2*4");
    long double result = eq.solution();
    EXPECT_EQ(result, -24);
}

TEST(Equation, OrderOfOperations2) {
    Equation eq("4+5+1-2+5*10-9");
    long double result = eq.solution();
    EXPECT_EQ(result, 49);
}

TEST(Equation, OrderOfOperations3) {
    Equation eq("3+5-16/2*4 + 4**2");
    long double result = eq.solution();
    EXPECT_EQ(result, -8);
}

TEST(Equation, InvalidOperation) {
    EXPECT_THROW({
        try {
            Equation eq("1 ? 2");
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ(e.what(), "Invalid Operation");
            throw;
        }
    }, std::invalid_argument);
}

TEST(Equation, MissingEquation) {
    EXPECT_THROW({
        try {
            Equation eq("");
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ(e.what(), "Missing Equation");
            throw;
        }
    }, std::invalid_argument);
}

TEST(Equation, MismatchedParentheses1) {
    EXPECT_THROW({
        try {
            Equation eq("((1+2)-1");
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ(e.what(), "Mismatched Parentheses");
            throw;
        }
    }, std::invalid_argument);
}

TEST(Equation, MismatchedParentheses2) {
    EXPECT_THROW({
        try {
            Equation eq("(1+2))-1");
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ(e.what(), "Mismatched Parentheses");
            throw;
        }
    }, std::invalid_argument);
}

TEST(Equation, UnexpectedEnd) {
    EXPECT_THROW({
        try {
            Equation eq("2+3-(");
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ(e.what(), "Unexpected End of Equation");
            throw;
        }
    }, std::invalid_argument);
}

TEST(Equation, InvalidNumber1) {
    EXPECT_THROW({
        try {
            Equation eq("2/-");
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ(e.what(), "Invalid Number");
            throw;
        }
    }, std::invalid_argument);
}

TEST(Equation, InvalidNumber2) {
    EXPECT_THROW({
        try {
            Equation eq("a20+5");
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ(e.what(), "Invalid Number");
            throw;
        }
    }, std::invalid_argument);
}

TEST(Equation, InvalidNumber3) {
    EXPECT_THROW({
        try {
            Equation eq("1.5.2");
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ(e.what(), "Invalid Number");
            throw;
        }
    }, std::invalid_argument);
}

TEST(Equation, DivideByZero) {

    EXPECT_THROW({
        Equation eq("1/0");
        try {
            eq.solution();
        } catch(const std::runtime_error& e) {
            EXPECT_STREQ(e.what(), "Divide by Zero");
            throw;
        }
    }, std::runtime_error);
}