#include <iostream>
#include <iterator>
#include <list>
#include <tuple>

#include "equation.cpp"

using namespace std;


int main(int argc, char *argv[]) {
    string equationString;
    if (argc > 1) {
        equationString = argv[1];
    } else {
        cout << "Input an equation. It will be evaluated using PEMDAS order of operations." << endl;
        cin >> equationString;
    }

    Equation eq(equationString);
    int result = eq.solution();
    cout << "Result = " << result << endl;
    return 0;
}