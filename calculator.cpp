#include <iostream>
#include <iterator>
#include <list>
#include <tuple>

using namespace std;

/**
* One operation in an equation, contains the value, the operation to be performed on the value to its right in the equation,
* and priority, which represents how many layers of parentheses deep this operation is
*/
class Operation {
    enum operationType { add, subtract, multiply, divide };
    public:
        int value; // the value at this operation
        operationType opType; // what will be performed on the next operation (addition, subtraction, etc)
        int priority; // how much priority this operation has, the greater the value the higher priority

        Operation(int value, char typeChar, int priority) {
            this->value = value;
            this->priority = priority;

            switch (typeChar) {
                case '+':
                    this->opType = add;
                    break;
                case '-':
                    this->opType = subtract;
                    break;
                case '*':
                    this->opType = multiply;
                    break;
                case '/':
                    this->opType = divide;
                    break;
            }

        }

        /**
        * After operations are performed, the value becoms the result of the operation,
        * and its operation type and priority will become that of what it operated on
        *
        * param - the operation value that this operation will be performed upon
        * minPriority - the minimum priority that this operation can proceed for
        * Returns true when the operation could be performed
        */
        bool performDivide(Operation param, int minPriority) {
            if (this->priority < minPriority) {
                return false;
            }

            if (this->opType == divide) {
                this->value = this->value / param.value;
                this->opType = param.opType;
                this->priority = param.priority;
                return true;
            }
            return false;
        }

        bool performMultiply(Operation param, int minPriority) {
            if (this->priority < minPriority) {
                return false;
            }

            if (this->opType == multiply) {
                this->value = this->value * param.value;
                this->opType = param.opType;
                this->priority = param.priority;
                return true;
            }
            return false;
        }

        bool performAddOrSub(Operation param, int minPriority) {
            if (this->priority < minPriority) {
                return false;
            }

            if (this->opType == add) {
                this->value = this->value + param.value;
                this->opType = param.opType;
                this->priority = param.priority;
                return true;
            }
            if (this->opType == subtract) {
                this->value = this->value - param.value;
                this->opType = param.opType;
                this->priority = param.priority;
                return true;
            }
            return false;
        }
};


class Equation {

    public:

        list<Operation> operations;
        int maxDepth = 0;

        Equation(string equationString) {
            std::tie(this->operations, this->maxDepth) = readEquation(equationString);
        }

        /**
        * Simplify the equation down to just the final result
        *
        * Returns the solution to the equation
        */
        int solution() {
            return computeOperations(operations, maxDepth);
        }

        /**
        * Read through an equation as a string and return a list of operations
        *
        * eq - the equation string to read
        * Returns a typle of a list of operations and the maximum depth of parentheses found
        */
        tuple<list<Operation>, int> readEquation(string eq) {
            list<Operation> ops;
            int length = eq.size();
            int depth = 0;
            int maxDepth = 0;

            for (int i = 0; i < length; i++) {

                while (i < length && eq[i] == ' ') { i++; } // skip spaces

                // increment our depth when we find an open parentheses
                while (i < length && eq[i] == '(') {
                    i++;
                    depth++;
                    if (depth > maxDepth) {
                        maxDepth = depth;
                    }
                }

                while (i < length && eq[i] == ' ') { i++; } // skip spaces

                string valueStr;
                valueStr += eq[i]; // always grab the number's first digit or minus sign
                i++;

                // find all the digits in a number
                while (i < length && eq[i] >= '0' && eq[i] <= '9') {
                    valueStr += eq[i];
                    i++;
                }

                while (i < length && eq[i] == ' ') { i++; } // skip spaces

                while (i < length && eq[i] == ')') {
                    i++;
                    depth--;
                }

                while (i < length && eq[i] == ' ') { i++; } // skip spaces

                // get our operation, default to addition for the last numbers's operation
                char typeChar = '+';
                if (i < length) {
                    typeChar = eq[i];
                }
                Operation op(stoi(valueStr), typeChar, depth);
                ops.push_back(op);

                while (i < length && eq[i] == ' ') { i++; } // skip spaces
            }


            return {ops, maxDepth};
    }

    /**
    * Iterates through operations starting at the highest priority (the deepest parentheses values)
    * At each level of depth, all multiplication operations, then division, and finally addition and subtraction
    * After an operation is able to be completed, its right partner will be deleted
    * This process repeats until only one operator containing the computed value remains
    *
    * ops - a list of operations
    * depth - the maximum depth to start looking at
    * returns the final result of computation of the operations
    */
    int computeOperations(list<Operation> &ops, int depth) {

        for (int priority = depth; priority >= 0; priority--) {

            // multiplication
            list<Operation>::iterator op = begin(ops);
            while ( op != end(ops) && next(op) != end(ops)) {
                if (op->performMultiply(*next(op), priority)) {
                    ops.erase(next(op));
                } else {
                    op++;
                }
            }

            // division
            op = begin(ops);
            while ( op != end(ops) && next(op) != end(ops)) {
                if (op->performDivide(*next(op), priority)) {
                    ops.erase(next(op));
                } else {
                    op++;
                }
            }

            // addition and subtraction
            op = begin(ops);
            while ( op != end(ops) && next(op) != end(ops)) {
                if (op->performAddOrSub(*next(op), priority)) {
                    ops.erase(next(op));
                } else {
                    op++;
                }
            }

        }

        return ops.begin()->value;
    }

};

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