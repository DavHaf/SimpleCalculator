#include <iostream>

#include "equation.cpp"

int main(int argc, char *argv[]) {
    std::string equationString;
    if (argc > 1) {
        equationString = argv[1];
    } else {
        std::cout << "Input an equation. It will be evaluated using PEMDAS order of operations." << std::endl;
        std::cin >> equationString;
    }

    Equation eq(equationString);
    int result = eq.solution();
    std::cout << "Result = " << result << std::endl;
    return 0;
}