#include <gtest/gtest.h>
#include <stdexcept>

#include "equation.cpp"

TEST(Equation, NoOperation) {
    Equation eq("7");
    int result = eq.solution();
    EXPECT_EQ(result, 7);
}

TEST(Equation, Addition) {
    Equation eq("2+2");
    int result = eq.solution();
    EXPECT_EQ(result, 4);
}

TEST(Equation, Subtraction) {
    Equation eq("1 - 3");
    int result = eq.solution();
    EXPECT_EQ(result, -2);
}

TEST(Equation, Multiplication) {
    Equation eq("3 * -4");
    int result = eq.solution();
    EXPECT_EQ(result, -12);
}

TEST(Equation, Division) {
    Equation eq("48/-3");
    int result = eq.solution();
    EXPECT_EQ(result, -16);
}

TEST(Equation, NegativeStart) {
    Equation eq("-1+5");
    int result = eq.solution();
    EXPECT_EQ(result, 4);
}

TEST(Equation, Parentheses0) {
    Equation eq("(1+4)");
    int result = eq.solution();
    EXPECT_EQ(result, 5);
}

TEST(Equation, Parentheses1) {
    Equation eq("6*(1+4)");
    int result = eq.solution();
    EXPECT_EQ(result, 30);
}

TEST(Equation, Parentheses2) {
    Equation eq("11 * (1+4) / (4*(2+4))");
    int result = eq.solution();
    EXPECT_EQ(result, 2);
}

TEST(Equation, Parentheses3) {
    Equation eq("1-(2+(3-(4+5)))");
    int result = eq.solution();
    EXPECT_EQ(result, 5);
}

TEST(Equation, Parentheses4) {
    Equation eq("(((1-2)+3)-4)+5");
    int result = eq.solution();
    EXPECT_EQ(result, 3);
}

TEST(Equation, OrderOfOperations1) {
    Equation eq("3+5-16/2*4");
    int result = eq.solution();
    EXPECT_EQ(result, 6);
}

TEST(Equation, OrderOfOperations2) {
    Equation eq("4+5+1-2+5*10-9");
    int result = eq.solution();
    EXPECT_EQ(result, 49);
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