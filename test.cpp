#include <gtest/gtest.h>

#include "equation.cpp"

TEST(QueueTest, Addition) {
  Equation eq("2+2");
  int result = eq.solution();
  EXPECT_EQ(result, 4);
}