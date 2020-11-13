#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct answerCalcInfix {
    double val;
    bool isSuccess;
};

struct answerCovertInfix {
    string val;
    bool isSuccess;
};

struct answerConvNumber {
    double val;
    bool isSuccess;
};

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

void standardizedExpression(string *s) {
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
            return 4;
            break;
        ////////////////
        case '*':
        case '/':
            return 3;
            break;
        ////////////////
        case '+':
        case '-':
            return 2;
            break;
        ////////////////
        case ')':
            return 1;
            break;
    }
    return 0;
}

bool isNum(string num) {
    bool dotted = false;
    for (int i = 0; i < num.length(); ++i) {
        if ((!isdigit(num[i]) && !num[i] == '.') ||
            num[i] == '.' && dotted)
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
    if (o == '^' || o == '*' || o == '/' || o == '+' || o == '-')
        return true;
    return false;
}

double convertNumber(string num) {
    if (!isNum(num))
        throw "Number invalid!";
    return stod(num);
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
    stack<double> operandStack;
    stack<char> operatorStack;
    while (!exp.empty()) {
        if (isblank(exp[0])) exp.erase(0, 1);
        if (isdigit(exp[0])) {
            size_t delim = exp.find(' ');
            double operand = convertNumber(exp.substr(0, delim));
            operandStack.push(operand);
            exp.erase(0, delim);
        }
        if (exp[0] == '(' || exp[0] == '[' || exp[0] == '{') {
            operatorStack.push(exp[0]);
            exp.erase(0, 1);
        }
        if (isOperator(exp[0])) {
            while (!operatorStack.empty() &&
                   precedence(operatorStack.top() >= precedence(exp[0]))) {
                double val2 = operandStack.top();
                operandStack.pop();
                double val1 = operandStack.top();
                operandStack.pop();
                char op = operatorStack.top();
                operatorStack.pop();
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
        operandStack.push(calculate(val1, val2, op));
    }
    return operandStack.top();
}

int main() {
    string exp;
    getline(cin, exp, '\n');
    standardizedExpression(&exp);
    double ans = calcInfix(exp);
    cout << ans << endl;
    return 0;
}