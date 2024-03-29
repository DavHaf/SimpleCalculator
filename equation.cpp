#include <algorithm>
#include <cmath>
#include <iterator>
#include <list>
#include <memory>
#include <stdexcept>

/**
* One operation in an equation, contains the value, the operation to be performed on the value to its right in the equation,
* and priority, which represents how many layers of parentheses deep this operation is
*/
class Operation {

    public:
        Operation(const long double& value, const int& priority) {
            this->value = value;
            this->priority = priority;
        }

        long double getValue() { return this->value; }

        /**
        * After operations are performed, the value becoms the result of the operation,
        * and its operation type and priority will become that of what it operated on
        *
        * param - the operation value that this operation will be performed upon
        * Returns true if the operation could be performed
        */
        bool calculate(std::unique_ptr<Operation>& param) {
            if (this->priority < param->priority) {
                return false;
            }

            if (this->priority == param->priority && this->getOpType() > param->getOpType()) {
                return false;
            }

            param->value = this->performOperation(param->value);
            return true;
        }

    protected:
        enum operationType { exponent, multiplyDivide, addSubtract }; // categories of operations starting with the highest order priority
        long double value; // the value at this operation
        int priority; // how much priority this operation has based on parentheses, the greater the value the higher priority

        virtual operationType getOpType() = 0;
        virtual long double performOperation(const long double& value) = 0;
};

class Add : public Operation {
    public:
        Add(const long double& value, const int& priority) : Operation(value, priority) {}
    private:
        operationType getOpType() override { return addSubtract; }

        long double performOperation(const long double& value) override {
            return this->value + value;
        }
};

class Subtract : public Operation {
    public:
        Subtract(const long double& value, const int& priority) : Operation(value, priority) {}
    private:
        operationType getOpType() override { return addSubtract; }

        long double performOperation(const long double& value) override {
            return this->value - value;
        }
};

class Multiply : public Operation {
    public:
        Multiply(const long double& value, const int& priority) : Operation(value, priority) {}
    private:
        operationType getOpType() override { return multiplyDivide; }

        long double performOperation(const long double& value) override {
            return this->value * value;
        }
};

class Divide : public Operation {
    public:
        Divide(const long double& value, const int& priority) : Operation(value, priority) {}
    private:
        operationType getOpType() override { return multiplyDivide; }

        long double performOperation(const long double& value) override {
            if (value == 0) {
                throw std::runtime_error( "Divide by Zero" );
            }
            return this->value / value;
        }
};

class DivideFloor : public Operation {
    public:
        DivideFloor(const long double& value, const int& priority) : Operation(value, priority) {}
    private:
        operationType getOpType() override { return multiplyDivide; }

        long double performOperation(const long double& value) override {
            if (value == 0) {
                throw std::runtime_error( "Divide by Zero" );
            }
            return std::floor(this->value / value);
        }
};

class Exponent : public Operation {
    public:
        Exponent(const long double& value, const int& priority) : Operation(value, priority) {}
    private:
        operationType getOpType() override { return exponent; }

        long double performOperation(const long double& value) override {
            return std::pow(this->value, value);
        }
};

class Equation {

    public:

        std::list<std::unique_ptr<Operation>> operations;

        Equation(std::string equationString) {
            // Remove Space Characters
            equationString.erase(remove_if(equationString.begin(), equationString.end(), isspace), equationString.end());
            this->operations = readEquation(equationString);
        }

        /**
        * Simplify the equation down to just the final result
        *
        * Returns the solution to the equation
        */
        long double solution() {
            computeOperations();
            return operations.front()->getValue();
        }

    private:

        /**
        * Read through an equation as a string and return a list of operations
        *
        * eq - the equation string to read
        * A list of operations
        */
        static std::list<std::unique_ptr<Operation>> readEquation(std::string eq) {
            std::list<std::unique_ptr<Operation>> ops;

            int depth = 0;
            int length = eq.size();

            if (length == 0) {
                throw std::invalid_argument( "Missing Equation" );
            }

            for (int i = 0; i < length; i++) {

                // increment our depth when we find an open parentheses
                while (i < length && eq[i] == '(') {
                    i++;
                    depth++;
                }
            
                if (i >= length) {
                    throw std::invalid_argument( "Unexpected End of Equation" );
                }


                std::string valueStr;
                // always grab the number's first digit or minus sign
                valueStr += eq[i];
                i++;

                int decimalCount = 0;

                // find all the digits in a number
                while (i < length && ((eq[i] >= '0' && eq[i] <= '9') || eq[i] == '.')) {

                    if (eq[i] == '.') {
                        decimalCount ++;
                    }
                    if (decimalCount > 1) {
                        throw std::invalid_argument( "Invalid Number");
                    }

                    valueStr += eq[i];
                    i++;
                }

                long double value;

                try {
                    value = stold(valueStr);
                } catch(const std::invalid_argument& e) {
                    throw std::invalid_argument( "Invalid Number" );
                }

                while (i < length && eq[i] == ')') {
                    i++;
                    depth--;
                    if (depth < 0) {
                        throw std::invalid_argument( "Mismatched Parentheses" );
                    }
                }

                // get our operation, default to addition for the last numbers's operation
                if (i >= length) {
                    ops.push_back(std::make_unique<Add>(value, depth));
                    break;
                }

                switch (eq[i]) {
                    case '+':
                        ops.push_back(std::make_unique<Add>(value, depth));
                        break;
                    case '-':
                        ops.push_back(std::make_unique<Subtract>(value, depth));
                        break;
                    case '*':
                        if (i < length && eq[i+1] == '*') {
                            ops.push_back(std::make_unique<Exponent>(value, depth));
                            i++;
                        } else {
                            ops.push_back(std::make_unique<Multiply>(value, depth));
                        }
                        break;
                    case '/':
                        if (i < length && eq[i+1] == '/') {
                            ops.push_back(std::make_unique<DivideFloor>(value, depth));
                            i++;
                        } else {
                            ops.push_back(std::make_unique<Divide>(value, depth));
                        }
                        break;
                    default:
                        throw std::invalid_argument( "Invalid Operation" );
                }
            }

            if (depth != 0) {
                throw std::invalid_argument( "Mismatched Parentheses" );
            }

            return ops;
    }

    /**
    * Reduce the list of operations down to just a single operation with the equation's solution as its value
    */
    void computeOperations() {
    
        std::list<std::unique_ptr<Operation>>& ops = this->operations;
        int numCalculated = 0;

        // repeat until not calculations are left to compute
        do {
            numCalculated = 0;

            // loop through operations and try to compute 
            for (auto op = begin(ops); op != end(ops) && next(op) != end(ops); op++) {
                // if the operation was allowed by order of operations, remove that operation from the list
                if ((*op)->calculate(*next(op))) {
                    op = ops.erase(op);
                    numCalculated++;
                }
            }
        } while (numCalculated > 0);
    }
};