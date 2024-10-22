#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <cctype>
#include <cmath>

// Function to check if a character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Function to get the precedence of operators
int precedence(char op) {
    switch(op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// Function to perform arithmetic operations
double applyOp(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b != 0) return a / b;
            throw std::invalid_argument("Division by zero");
        case '^': return pow(a, b);
        default: return 0;
    }
}

// Function to evaluate a simple expression with only addition, subtraction, multiplication, and division
double evaluateExpression(const std::string &expr) {
    std::stack<double> values;
    std::stack<char> ops;

    for (size_t i = 0; i < expr.length(); i++) {
        // Skip spaces
        if (expr[i] == ' ') continue;

        // If the current character is a number, parse the number
        if (isdigit(expr[i])) {
            double val = 0;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') {
                    double decimal_place = 10;
                    ++i;
                    while (i < expr.length() && isdigit(expr[i])) {
                        val += (expr[i] - '0') / decimal_place;
                        decimal_place *= 10;
                        ++i;
                    }
                    --i; // To counteract the extra increment in the loop
                } else {
                    val = val * 10 + (expr[i] - '0');
                }
                ++i;
            }
            --i; // To counteract the extra increment in the loop
            values.push(val);
        } 
        // If the current character is an opening parenthesis
        else if (expr[i] == '(') {
            ops.push(expr[i]);
        } 
        // If the current character is a closing parenthesis
        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            if (!ops.empty()) ops.pop(); // Remove '(' from stack
        } 
        // If the current character is an operator
        else if (isOperator(expr[i])) {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(expr[i]);
        }
    }

    while (!ops.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.top();
}

int main() {
    std::string expression;
    std::cout << "Enter an arithmetic expression (use spaces between operators and operands): " << std :: endl;
    std::getline(std::cin, expression);

    try {
        double result = evaluateExpression(expression);
        std::cout << "The result of the expression is: " << result << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
