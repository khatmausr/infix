#include <fstream>
#include <iostream>
#include <iomanip>
#include <stack>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

bool isNum(string num) {
    if (num.back() == '.' || num.front() == '.') return false;
    bool dotted = false;
    for (int i = 0; i < num.length(); ++i) {
        if ((!isdigit(num[i]) && !(num[i] == '.')) ||
            (num[i] == '.' && dotted))
            return false;
        if (num[i] == '.') {
            if (i < num.length() - 1 - 2)
                return false;
            dotted = true;
        }
    }
    return true;
}

bool isOperator(char o) {
    return (o == '^' || o == '*' || o == '/' || o == '+' || o == '-');
}

bool isBracket(char o) {
    return (o == '(' || o == ')' || o == '[' || o == ']' || o == '{' || o == '}');
}

vector<string> readExp(string input_file, unsigned int limit) {
    ifstream fin(input_file);
    vector<string> exp;
    unsigned int linesCounter = 0;
    while (linesCounter < limit && !fin.eof()) {
        string line;
        getline(fin, line, '\n');
        exp.push_back(line);
    }
    fin.close();
    return exp;
}

void standardizedExpression(string* s) {
    while (s->at(0) == ' ') s->erase(0, 1);
    while (s->at(s->length() - 1) == ' ') s->erase(s->length() - 1, 1);
    for (int i = 0; i < s->length() - 1; ++i) {
        while (s->at(i) == ' ' && s->at(i + 1) == ' ') s->erase(i, 1);
        while (s->at(i) == '(' && s->at(i + 1) == ' ') s->erase(i + 1, 1);
        while (s->at(i) == ' ' && s->at(i + 1) == ')') s->erase(i, 1);
    }
}

unsigned short precedence(char o) {
    switch (o) {
        case '^':
            return 3;
            break;
        ////////////////
        case '*':
        case '/':
            return 2;
            break;
        ////////////////
        case '+':
        case '-':
            return 1;
            break;
    }
    return 0;
}

double convertNumber(string num) {
    if (!isNum(num))
        throw "Number invalid!";
    return stod(num);
}

bool isPairBracket(char openB, char closeB) {
    if ((closeB == ')' && openB != '(') ||
        (closeB == ']' && openB != '[') ||
        (closeB == '}' && openB != '{'))
        return false;
    return true;
}

double calculate(double val1, double val2, char op) {
    switch (op) {
        case '^':
            return pow(val1, val2);
            break;
        case '*':
            return val1 * val2;
            break;
        case '/':
            if (val2 == 0)
                throw "Divide by zero!";
            else
                return val1 / val2;
            break;
        case '+':
            return val1 + val2;
            break;
        case '-':
            return val1 - val2;
            break;
    }
    return 0;
}

//32.34
double calcInfix(string exp) {
    char lastOp = 0;
    stack<double> operandStack;
    stack<char> operatorStack;
    while (!exp.empty()) {
        if (isblank(exp[0])) exp.erase(0, 1);
        if (isdigit(exp[0])) {
            if (lastOp == 1) throw "Infix invalid: Double number catched!";
            lastOp = 1;
            int delim = 0;
            while (isdigit(exp[delim]) || exp[delim] == '.') ++delim;
            double operand = convertNumber(exp.substr(0, delim));
            operandStack.push(operand);
            exp.erase(0, delim);
        }
        if (exp[0] == '(' || exp[0] == '[' || exp[0] == '{') {
            operatorStack.push(exp[0]);
            exp.erase(0, 1);
        }
        if (exp[0] == ')' || exp[0] == ']' || exp[0] == '}') {
            while (!operatorStack.empty() && !isBracket(operatorStack.top())) {
                double val2 = operandStack.top();
                operandStack.pop();
                double val1 = operandStack.top();
                operandStack.pop();
                char op = operatorStack.top();
                operatorStack.pop();
                if (!isOperator(op))
                    throw "Invalid operator";
                else
                    operandStack.push(calculate(val1, val2, op));
            }
            char bracket = operatorStack.top();
            operatorStack.pop();
            if (!isPairBracket(bracket, exp[0])) throw "Bracket order invalid!";
            exp.erase(0, 1);
        }
        if (isOperator(exp[0])) {
            if (lastOp == -1) throw "Infix invalid: Double operator catched!";
            lastOp = -1;
            while (!operatorStack.empty() &&
                   precedence(operatorStack.top()) >= precedence(exp[0])) {
                double val2 = operandStack.top();
                operandStack.pop();
                double val1 = operandStack.top();
                operandStack.pop();
                char op = operatorStack.top();
                operatorStack.pop();
                if (!isOperator(op))
                    throw "Invalid operator";
                else
                    operandStack.push(calculate(val1, val2, op));
            }
            operatorStack.push(exp[0]);
            exp.erase(0, 1);
        }
    }
    while (!operatorStack.empty()) {
        double val2 = operandStack.top();
        operandStack.pop();
        double val1 = operandStack.top();
        operandStack.pop();
        char op = operatorStack.top();
        operatorStack.pop();
        if (!isOperator(op))
            throw "Invalid operator";
        else
            operandStack.push(calculate(val1, val2, op));
    }
    return operandStack.top();
}

string infix2Postfix(string exp) {
    char lastOp = 0;
    string ans = "";
    stack<char> operatorStack;
    while (!exp.empty()) {
        if (isblank(exp[0])) exp.erase(0, 1);
        if (isdigit(exp[0])) {
            if (lastOp == 1) throw "Infix invalid: Double number catched!";
            lastOp = 1;
            int delim = 0;
            while (isdigit(exp[delim]) || exp[delim] == '.') ++delim;
            string num = exp.substr(0, delim);
            if (!isNum(num))
                throw "Number invalid";
            else
                ans += num + " ";
            exp.erase(0, delim);
        }
        if (exp[0] == '(' || exp[0] == '[' || exp[0] == '{') {
            operatorStack.push(exp[0]);
            exp.erase(0, 1);
        }
        if (exp[0] == ')' || exp[0] == ']' || exp[0] == '}') {
            while (!operatorStack.empty() && !isBracket(operatorStack.top())) {
                char op = operatorStack.top();
                operatorStack.pop();
                if (!isOperator(op))
                    throw "Invalid operator";
                else
                    ans = ans + op + " ";
            }
            char bracket = operatorStack.top();
            operatorStack.pop();
            if (!isPairBracket(bracket, exp[0])) throw "Bracket order invalid!";
            exp.erase(0, 1);
        }
        if (isOperator(exp[0])) {
            if (lastOp == -1) throw "Infix invalid: Double operator catched!";
            lastOp = -1;
            while (!operatorStack.empty() &&
                   precedence(operatorStack.top()) > precedence(exp[0])) {
                char op = operatorStack.top();
                operatorStack.pop();
                if (!isOperator(op))
                    throw "Invalid operator";
                else
                    ans = ans + op + " ";
            }
            operatorStack.push(exp[0]);
            exp.erase(0, 1);
        }
    }
    while (!operatorStack.empty()) {
        char op = operatorStack.top();
        operatorStack.pop();
        if (!isOperator(op))
            throw "Invalid operator";
        else
            ans = ans + op + " ";
    }
    return ans;
}
int main() {
    string exp = "15 ^ (20 / (5 * 20) + 10";
    //getline(fin, exp, '\n');
    try {
        standardizedExpression(&exp);
        // double ans = calcInfix(exp);
        string ans = infix2Postfix(exp);
        cout << ans;
    } catch (char const* e) {
        cout << e;
    }

    return 0;
}