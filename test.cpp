#include <gtest/gtest.h>

#include "equation.cpp"

TEST(QueueTest, Addition) {
  Equation eq("2+2");
  int result = eq.solution();
  EXPECT_EQ(result, 4);
}

TEST(QueueTest, Subtraction) {
  Equation eq("1 - 3");
  int result = eq.solution();
  EXPECT_EQ(result, -2);
}

TEST(QueueTest, Multiplication) {
  Equation eq("3 * -4");
  int result = eq.solution();
  EXPECT_EQ(result, -12);
}

TEST(QueueTest, Division) {
  Equation eq("48/-3");
  int result = eq.solution();
  EXPECT_EQ(result, -16);
}

TEST(QueueTest, NegativeStart) {
  Equation eq("-1+5");
  int result = eq.solution();
  EXPECT_EQ(result, 4);
}

TEST(QueueTest, Parentheses0) {
  Equation eq("(1+4)");
  int result = eq.solution();
  EXPECT_EQ(result, 5);
}

TEST(QueueTest, Parentheses1) {
  Equation eq("6*(1+4)");
  int result = eq.solution();
  EXPECT_EQ(result, 30);
}

TEST(QueueTest, Parentheses2) {
  Equation eq("11*(1+4)/(4*(2+4))");
  int result = eq.solution();
  EXPECT_EQ(result, 2);
}

TEST(QueueTest, Parentheses3) {
  Equation eq("1-(2+(3-(4+5)))");
  int result = eq.solution();
  EXPECT_EQ(result, 5);
}

TEST(QueueTest, Parentheses4) {
  Equation eq("(((1-2)+3)-4)+5");
  int result = eq.solution();
  EXPECT_EQ(result, 3);
}

TEST(QueueTest, OrderOfOperations1) {
  Equation eq("3+5-16/2*4");
  int result = eq.solution();
  EXPECT_EQ(result, 6);
}

TEST(QueueTest, OrderOfOperations2) {
  Equation eq("4+5+1-2+5*10-9");
  int result = eq.solution();
  EXPECT_EQ(result, 49);
}