#ifndef POSTFIXEVAL_H
#define POSTFIXEVAL_H

double postfixeval(String input);
String infixtopostfix(String fxn);
bool isOperator(char C);
bool isOperand(char C);
bool isLeftBracket(char C);
bool isRightBracket(char C);
bool isRightAssociative(char operatorr);
int getOperatorWeight (char operatorr);
bool ownHigherPrecedence(char operatorr1, char operatorr2);
bool flag(char C);
double domath(char operation, double operand1, double operand2);
bool isBalanced(String fxn);
#endif
