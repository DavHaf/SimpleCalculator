#include <iostream>

#include "equation.cpp"

int main(int argc, char *argv[]) {
    std::string equationString;
    if (argc > 1) {
        equationString = argv[1];
    } else {
        std::cout << "Input an equation. It will be evaluated parentheses first, then exponentiation, division and multiplication, and finally addition and subtraction." << std::endl;
        std::cin >> equationString;
    }

    try {
        Equation eq(equationString);
        long double result = eq.solution();
        std::cout << "Result = " << result << std::endl;
    } catch(const std::invalid_argument& e) {
        std::cout << "Error parsing equation: " << e.what() << std::endl;
        return -1;
    } catch(const std::runtime_error& e) {
        std::cout << "Error processing equation: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}