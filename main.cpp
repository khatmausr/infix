#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

/// function isNum
/// Check whether input string is a number or not.
/// Return true if input string is a number or false, vice versa.
bool isNum(string num) {
    if (num.back() == '.' || num.front() == '.') return false;  // Check whether there is dot at the front/back of the string
    bool dotted = false;
    for (int i = 0; i < num.length(); ++i) {
        if ((!isdigit(num[i]) && !(num[i] == '.')) ||  // Whether a char is not digit or a dot(.)
            (num[i] == '.' && dotted))                 // There are 2 dots in the string
            return false;
        if (num[i] == '.') {  // Input number with number of decimal digit > 2
            if (i < num.length() - 1 - 2)
                return false;
            dotted = true;
        }
    }
    return true;
}

/// function isOperator
/// Check whether input char is an operator
/// return true if input char is an operator or false, vice versa
bool isOperator(char o) {
    return (o == '^' || o == '*' || o == '/' || o == '+' || o == '-');
}

/// function isBracket
/// Check whether input char is an bracket
/// return true if input char is an bracket or false, vice versa
bool isBracket(char o) {
    return (o == '(' || o == ')' || o == '[' || o == ']' || o == '{' || o == '}');
}

/// function isStandardizedExpression
/// Check whether input expression is valid
/// return true if input expression is valid or false, vice versa
void isStandardizedExpression(string s) {
    if (s.front() == ' ' || s.back() == ' ') throw "Invalid expression: front | back";  // Check if
    for (int i = 1; i < s.length() - 1; ++i) {
        if (isOperator(s[i]) && (s[i - 1] != ' ' || s[i + 1] != ' ')) throw "Invalid expression: spaces around operator";
        if (s[i] == ' ' && s[i + 1] == ' ') throw "Invalid expression: spaces";
    }
}

unsigned short bracketPrecedence(char o) {
    if (!isBracket(o)) return 0;
    switch (o) {
        case '{':
        case '}':
            return 3;
            break;
        case '[':
        case ']':
            return 2;
            break;
        case '(':
        case ')':
            return 1;
            break;
    }
    return 0;
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

double calcInfix(string exp) {
    bool lastOp = false;
    stack<double> operandStack;
    stack<char> bracketStack;
    stack<char> operatorStack;
    while (!exp.empty()) {
        if (isblank(exp[0])) exp.erase(0, 1);
        if (isdigit(exp[0])) {
            if (lastOp) throw "Infix invalid: Double number catched!";
            lastOp = true;
            int delim = 0;
            while (isdigit(exp[delim]) || exp[delim] == '.') ++delim;
            double operand = convertNumber(exp.substr(0, delim));
            operandStack.push(operand);
            exp.erase(0, delim);
        }
        if (exp[0] == '(' || exp[0] == '[' || exp[0] == '{') {
            operatorStack.push(exp[0]);
            if (!bracketStack.empty() && bracketPrecedence(exp[0]) >= bracketPrecedence(bracketStack.top()))
                throw "Invalid bracket order!";
            bracketStack.push(exp[0]);
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
            char bracket = bracketStack.top();
            operatorStack.pop();
            bracketStack.pop();
            if (!isPairBracket(bracket, exp[0])) throw "Bracket pair invalid!";
            exp.erase(0, 1);
        }
        if (isOperator(exp[0])) {
            if (!lastOp) throw "Infix invalid: Double operator catched!";
            lastOp = false;
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
    bool lastOp = false;
    string ans = "";
    stack<char> bracketStack;
    stack<char> operatorStack;
    while (!exp.empty()) {
        if (isblank(exp[0])) exp.erase(0, 1);
        if (isdigit(exp[0])) {
            if (lastOp) throw "Infix invalid: Double number catched!";
            lastOp = true;
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
            if (!bracketStack.empty() && bracketPrecedence(exp[0]) >= bracketPrecedence(bracketStack.top()))
                throw "Invalid bracket order!";
            bracketStack.push(exp[0]);
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
            char bracket = bracketStack.top();
            operatorStack.pop();
            bracketStack.pop();
            if (!isPairBracket(bracket, exp[0])) throw "Bracket order invalid!";
            exp.erase(0, 1);
        }
        if (isOperator(exp[0])) {
            if (!lastOp) throw "Infix invalid: Double operator catched!";
            lastOp = false;
            while (!operatorStack.empty() &&
                   precedence(operatorStack.top()) >= precedence(exp[0])) {
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
void test(string exp){
    try {
        isStandardizedExpression(exp);
        double ans = calcInfix(exp);
        cout << setprecision(2) << ans << endl;
    } catch (char const* e) {
        cout << "E\n";
    }
}

void exec(char* argv[]){
    ifstream fin(argv[1]);
    ofstream fout(argv[4]);
    int n = atoi(argv[2]);
    for (int i = 0; i < n; ++i) {
        string exp;
        if (getline(fin, exp, '\n')) {
            try {
                isStandardizedExpression(exp);
                if (!strcmp(argv[3], "-c")) {
                    double ans = calcInfix(exp);
                    fout << setprecision(2) << ans << endl;
                } else if (!strcmp(argv[3], "-t")) {
                    string ans = infix2Postfix(exp);
                    fout << ans << endl;
                }
            } catch (char const* e) {
                fout << "E\n";
            }
        }
    }
    fin.close();
    fout.close();
}
int main(int argc, char* argv[]) {
    //test("(5 + 5}");
    exec(argv);
    return 0;
}