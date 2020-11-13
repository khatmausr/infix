#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "stdio.h"
#include "stdlib.h"

using namespace std;

/// function isNum
/// Check whether input string is a number or not.
/// Return true if input string is a number or false, vice versa.
bool isNum(string num) {
    if (num[num.length() - 1] == '.' || num[0] == '.') return false;  // Check whether there is dot at the front/back of the string
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
    if (!(isdigit(s[0]) || s[0] == '(' || s[0] == '[' || s[0] == '{')) throw "Invalid expression: front";  // expression has spaces at the front/back
    if (!(isdigit(s[s.length() - 1]) || s[s.length() - 1] == ')' || s[s.length() - 1] == ']' || s[s.length() - 1] == '}')) throw "Invalid expression: back";
    for (int i = 1; i < s.length() - 1; ++i) {
        if (!isdigit(s[i]) && !isBracket(s[i]) && !isOperator(s[i]) && s[i] != '.' && !isblank(s[i]))
            throw "Invalid expression";
        if (i > 0 && i < s.length() - 1) {
            if ((s[i] == '(' || s[i] == '[' || s[i] == '{') && !isdigit(s[i + 1])) throw "Invalid bracket - digit";
            if ((s[i] == ')' || s[i] == ']' || s[i] == '}') && !isdigit(s[i - 1])) throw "Invalid digit - bracket";
            if (isOperator(s[i]) && (!isblank(s[i - 1]) || !isblank(s[i + 1])))  // no space around operator
                throw "Invalid expression: spaces around operator";
            if (isblank(s[i]) && isblank(s[i + 1])) throw "Invalid expression: spaces";  // double spaces '  '
        }
    }
}

/// function bracketPrecedence
/// Prioritize the bracket () -> [] -> {}
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

/// function precedence
/// Prioritize the operator ^ -> *, / -> +, -
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

/// function convertNumber
/// conver a string to number
/// throw error when input is not a number
/// return a double value converted from string
double convertNumber(string num) {
    if (!isNum(num))
        throw "Number invalid!";
    return atof(num.c_str());
}

/// function isPairBracket
/// check 2 char is a pair of bracket or not
bool isPairBracket(char openB, char closeB) {
    if ((closeB == ')' && openB != '(') ||
        (closeB == ']' && openB != '[') ||
        (closeB == '}' && openB != '{'))
        return false;
    return true;
}

/// function calculate
/// calculate 2 values with operator inputed
double calculate(double val1, double val2, char op) {
    switch (op) {
        case '^':
            if(val1 < 0 && (val2 > -1 && val2 < 1))
                throw "Wrong pow operation";
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

/// function calcInfix
/// calculate infix expression
double calcInfix(string exp) {
    bool lastOp = false;  // true - operands / false - operator / default - false - make sure that we meet operands before operator
    stack<double> operandStack;
    stack<char> bracketStack;
    stack<char> operatorStack;
    while (!exp.empty()) {
        if (!isdigit(exp[0]) && !isBracket(exp[0]) && !isOperator(exp[0]) && !isblank(exp[0]))
            throw "Invalid character!";
        if (isblank(exp[0])) exp.erase(0, 1);                           // delete space
        if (isdigit(exp[0])) {                                          // meet a number
            if (lastOp) throw "Infix invalid: Double number catched!";  // 2 operands next to each other (5 5 + 5)
            lastOp = true;
            int delim = 0;
            while (isdigit(exp[delim]) || exp[delim] == '.') ++delim;  // find every characters of the number
            double operand = convertNumber(exp.substr(0, delim));      // convert it into double
            operandStack.push(operand);                                // push to operand stack
            exp.erase(0, delim);                                       // erase it
        }
        if (exp[0] == '(' || exp[0] == '[' || exp[0] == '{') {  // meet an open bracket
            operatorStack.push(exp[0]);                         // push to operator stack
            if (!bracketStack.empty() &&
                bracketPrecedence(exp[0]) >= bracketPrecedence(bracketStack.top()))  // inner bracket must lower precedence than the outter bracket
                throw "Invalid bracket order!";
            bracketStack.push(exp[0]);  //
            exp.erase(0, 1);
        }
        if (exp[0] == ')' || exp[0] == ']' || exp[0] == '}') {                   // meet a close operator
            while (!operatorStack.empty() && !isBracket(operatorStack.top())) {  // calculate all expression inside the bracket
                double val2 = operandStack.top();                                // take 2 value in operand stack
                operandStack.pop();
                double val1 = operandStack.top();
                operandStack.pop();
                char op = operatorStack.top();
                operatorStack.pop();
                if (!isOperator(op))  // valid operator
                    throw "Invalid operator";
                else
                    operandStack.push(calculate(val1, val2, op));  // calculate 2 val with operator
            }
            if ((!bracketStack.empty() && !isPairBracket(bracketStack.top(), exp[0])) ||  // valid open-close bracket
                !isPairBracket(operatorStack.top(), exp[0])) throw "Bracket pair invalid!";
            bracketStack.pop();
            operatorStack.pop();
            exp.erase(0, 1);
        }
        if (isOperator(exp[0])) {  // meet an operator, do like above
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
    while (!operatorStack.empty()) {  // calculate all existing operators/operands in stack
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
    return operandStack.top();  // the answer is the operand top element in stack
}

/// function infix2Postfix
/// input an infix expression
/// convert to postfix expression
/// algorithm nearly like function calcInfix()
string infix2Postfix(string exp) {
    bool lastOp = false;
    string ans = "";
    stack<char> bracketStack;
    stack<char> operatorStack;
    while (!exp.empty()) {
        if (!isdigit(exp[0]) && !isBracket(exp[0]) && !isOperator(exp[0]) && !isblank(exp[0]))
            throw "Invalid character!";
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
            if (!isPairBracket(bracketStack.top(), exp[0]) || !isPairBracket(operatorStack.top(), exp[0])) throw "Bracket pair invalid!";
            bracketStack.pop();
            operatorStack.pop();
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
void test(string exp) {
    try {
        isStandardizedExpression(exp);
        double ans = calcInfix(exp);
        cout << setprecision(2) << ans << endl;
    } catch (char const* e) {
        cout << e << endl;
    }
}

void testfile(char* option) {
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    int n = 10;
    for (int i = 0; i < n; ++i) {
        string exp;
        if (getline(fin, exp, '\n')) {
            try {
                isStandardizedExpression(exp);
                if (!strcmp(option, "-c")) {
                    double ans = calcInfix(exp);
                    fout << setprecision(2) << ans << endl;
                } else if (!strcmp(option, "-t")) {
                    string ans = infix2Postfix(exp);
                    fout << ans << endl;
                }
            } catch (char const* e) {
                fout << e << endl;
            }
        }
    }
    fin.close();
    fout.close();
}

void exec(char* argv[]) {
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
    //test("7 ^ 2 + (5 ^ 8)");
    exec(argv);
    return 0;
}