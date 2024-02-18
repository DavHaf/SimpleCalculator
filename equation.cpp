#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <tuple>

using namespace std;

/**
* One operation in an equation, contains the value, the operation to be performed on the value to its right in the equation,
* and priority, which represents how many layers of parentheses deep this operation is
*/
class Operation {
    public:
        enum operationType { multiply, divide, addAndSub, last };
        int value; // the value at this operation
        operationType opType; // what will be performed on the next operation (addition, subtraction, etc)
        int priority; // how much priority this operation has, the greater the value the higher priority

        Operation(const int &value, const int &priority) {
            this->value = value;
            this->priority = priority;
        }

        virtual operationType getOpType() { return last; };
        virtual int performOperation(const int &value) {
            return this->value;
        };

        /**
        * After operations are performed, the value becoms the result of the operation,
        * and its operation type and priority will become that of what it operated on
        *
        * param - the operation value that this operation will be performed upon
        * minPriority - the minimum priority that this operation can proceed for
        * Returns true when the operation could be performed
        */
        bool calculate(Operation &param) {
            if (this->priority < param.priority) {
                return false;
            }

            if (this->priority == param.priority && this->getOpType() > param.getOpType()) {
                return false;
            }

            param.value = this->performOperation(param.value);
            return true;
        }
};

class Add : public Operation {
    public:
        Add(const int &value, const int &priority) : Operation(value, priority) {}
        operationType getOpType() override { return addAndSub; }

        int performOperation(const int &value) override {\
            return this->value + value;
        }
};

class Subtract : public Operation {
    public:
        Subtract(const int &value, const int &priority) : Operation(value, priority) {}
        operationType getOpType() override { return addAndSub; }

        int performOperation(const int &value) override {
            return this->value - value;
        }
};

class Multiply : public Operation {
    public:
        Multiply(const int &value, const int &priority) : Operation(value, priority) {}
        operationType getOpType() override { return multiply; }

        int performOperation(const int &value) override {
            return this->value * value;
        }
};

class Divide : public Operation {
    public:
        Divide(const int &value, const int &priority) : Operation(value, priority) {}
        operationType getOpType() override { return divide; }

        int performOperation(const int &value) override {
            return this->value / value;
        }
};

class Equation {

    public:

        list<unique_ptr<Operation>> operations;
        int maxDepth = 0;

        Equation(string equationString) {
            this->operations = readEquation(equationString);
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
        list<unique_ptr<Operation>> readEquation(string eq) {
            list<unique_ptr<Operation>> ops;
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

                int value = stoi(valueStr);

                switch (typeChar) {
                    case '-':
                        ops.push_back(make_unique<Subtract>(value, depth));
                        break;
                    case '*':
                        ops.push_back(make_unique<Multiply>(value, depth));
                        break;
                    case '/':
                        ops.push_back(make_unique<Divide>(value, depth));
                        break;
                    default:
                        ops.push_back(make_unique<Add>(value, depth));
                        break;

                }

                while (i < length && eq[i] == ' ') { i++; } // skip spaces
            }


            return ops;
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
    int computeOperations(list<unique_ptr<Operation>> &ops, int depth) {

        int numCalculated = 0;

        do {
            numCalculated = 0;
            auto op = begin(ops);
            while ( op != end(ops) && next(op) != end(ops)) {
                if ((*op)->calculate(**next(op))) {
                    op = ops.erase(op);
                    numCalculated++;
                }
                op++;
            }
        } while (numCalculated > 0);

        return (*ops.begin())->value;
    }
};