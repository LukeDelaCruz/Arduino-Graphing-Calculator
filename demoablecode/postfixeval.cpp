// Course: CMPUT 274 Fa17

#include <Arduino.h>
#include <string.h>
#include "stack.exercise.8.h"
#include "postfixeval.h"

// given the postfix string, we can evaluate the expression cam already handle more than one char!
// O(nlog(n))
double postfixeval(String input) {
	Stack s; // stack declaration from the same code used in exercise 8

	// markers that identify which functions are being used
	bool negflag = false;
	bool tanflag = false; bool sinflag = false; bool cosflag = false; bool expflag = false; bool logflag = false; // natural log
	bool atanflag = false; bool acosflag = false; bool asinflag = false;
	bool tanhflag = false; bool coshflag = false; bool sinhflag = false;

	// for loop string analysis
	for (unsigned int i = 0; i < input.length(); i++) {

		String fxnsubstr = input.substring(i, i+3+1); // 3 letter functions
		String fxnsubstr2 = input.substring(i, i+4+1);// 4 letter functions

		if (input.charAt(i) == ' ' || input.charAt(i) == ',') {continue;} // unwanted tokens or delimitter
		if (isLeftBracket(input.charAt(i)) && input.charAt(i+1) == '-') {
			i++; // only one increment is needed since the for loop will perform the other one
			negflag = true;
		}
		else if (isOperator(input.charAt(i))) {
			// Temporarily store then
			double operand2 = s.top(); s.pop();
			double operand1 = s.top(); s.pop();
			double result = domath(input.charAt(i), operand1, operand2);
			// New result is now on top of the stack
			s.push(result);
		}
		else if (isOperand(input.charAt(i))) {
			// search for numbers with multiple digits
			double operand = 0;
			int negpowoften = 1; // the fact that we're in that 2nd while loop means that we need already have a tenths place decimal
			while (i < input.length() && isOperand(input.charAt(i))) {
				// a new digit towards the right must be added to the number in it's entirety via decimal notation
				// tranlate the number to it's decimal form (i.e., '1' - '0' = 49 - 48 = 1 using ASCII)
				operand = (operand*10) + (input.charAt(i) - '0');
				i++;
				// handling tricky decimals...https://www.arduino.cc/reference/en/language/variables/data-types/float/
				if (input.charAt(i) == '.') {
					i++;
					while (i < input.length() && isOperand(input.charAt(i))) {
						operand += (double)(input.charAt(i) - '0') / pow(10, negpowoften);
						negpowoften++;
						i++;
					}
				}
			}
			// to keep checking you must increment i, but when you exit the while loop you're either at an
			// operator or the end of the string...now since i was incremented to check for checking purposes
			// it must be decremented once to ovoid a double incrementation (recall that we're in a for loop!)
			i--;

			/* more complex math*/
			if (negflag) {operand = (double)-1.0*operand; negflag = false;}
			if (tanflag) {operand = tan(operand); tanflag = false;}
			if (sinflag) {operand = sin(operand); sinflag = false;}
			if (cosflag) {operand = cos(operand); cosflag = false;}
			if (expflag) {operand = exp(operand); expflag = false;}
			if (logflag) {operand = log(operand); logflag = false;}
			if (atanflag) {operand = atan(operand); atanflag = false;}
			if (acosflag) {operand = acos(operand); acosflag = false;}
			if (asinflag) {operand = asin(operand); asinflag = false;}
			if (tanhflag) {operand = tanh(operand); tanhflag = false;}
			if (coshflag) {operand = cosh(operand); coshflag = false;}
			if (sinhflag) {operand = sinh(operand); sinhflag = false;}

			s.push(operand);
		}

		else if (fxnsubstr == "tan(" || fxnsubstr == "sin(" || fxnsubstr == "cos(" || fxnsubstr == "exp(") {
			if (fxnsubstr == "tan(") {tanflag = true;}
			else if (fxnsubstr == "sin(") {sinflag = true;}
			else if (fxnsubstr == "cos(") {cosflag = true;}
			else if (fxnsubstr == "exp(") {expflag = true;}
			else if (fxnsubstr == "log(") {logflag = true;}
			i += 4;
			int replacetracker = i-1; // after the replace we need to know where we are the string position
			CharStack bracketscollector;
			bracketscollector.CSpush('(');
			String nestedfxn = "(";
			char storage[50] = {0};
			while (!bracketscollector.isCSEmpty()) {
				if (isLeftBracket(input.charAt(i))) {
					bracketscollector.CSpush('(');
					nestedfxn += input.charAt(i);
					i++;
				}
				else if (isRightBracket(input.charAt(i))) {
					bracketscollector.CSpop();
					nestedfxn += input.charAt(i);
					i++;
				}
				else {
					nestedfxn += input.charAt(i);
					i++;
				}
			}
			dtostrf(postfixeval(nestedfxn), 1, 3, storage); // recursion and string conversion

			String tempstringstorage = String(storage);
			tempstringstorage = String("((" + tempstringstorage + "))");

			input.replace(nestedfxn, tempstringstorage); // then appension
			i = replacetracker; // you leave the for loop and it increments again so that i becomes 4
		}

		else if (fxnsubstr2 == "atan(" || fxnsubstr2 == "acos(" || fxnsubstr2 == "asin("
					|| fxnsubstr2 == "tanh(" || fxnsubstr2 == "cosh(" || fxnsubstr2 == "sinh(")
			{
			if (fxnsubstr2 == "atan(") {atanflag = true;}
			else if (fxnsubstr2 == "acos(") {acosflag = true;}
			else if (fxnsubstr2 == "asin(") {asinflag = true;}
			else if (fxnsubstr2 == "tanh(") {tanhflag = true;}
			else if (fxnsubstr2 == "cosh(") {coshflag = true;}
			else if (fxnsubstr2 == "sinh(") {sinhflag = true;}
			i += 5;
			int replacetracker = i-1; // after the replace we need to know where we were at the string position which is the first open bracket of the "mother function"
			CharStack bracketscollector;
			bracketscollector.CSpush('(');
			String nestedfxn = "(";
			char storage[50] = {0};
			while (!bracketscollector.isCSEmpty()) {
				if (isLeftBracket(input.charAt(i))) {
					bracketscollector.CSpush('(');
					nestedfxn += input.charAt(i);
					i++;
				}
				else if (isRightBracket(input.charAt(i))) {
					bracketscollector.CSpop();
					nestedfxn += input.charAt(i);
					i++;
				}
				else {
					nestedfxn += input.charAt(i);
					i++;
				}
			}
			dtostrf(postfixeval(nestedfxn), 1, 3, storage); // recursion and string conversion

			String tempstringstorage = String(storage);
			tempstringstorage = String("((" + tempstringstorage + "))");

			input.replace(nestedfxn, tempstringstorage); // then appension
			i = replacetracker; // you leave the for loop and it increments again so that i becomes 4
		}
	}
	// stack will now have one item remaining, such that it wil be the output
	return s.top();
}

// function that will convert an infix string to a postfix one, can't handle more than char!
// O(nlog(n))
String infixtopostfix(String fxn) {
	CharStack CS;
	String postfix = "";

	// parsing the expression only once, from left to right
	for (int i = 0; i < fxn.length(); i++) {

		/*---handling some of the special 3 (+1 left bracket inclusive) character functions---*/
		String fxnsubstr = fxn.substring(i, i+3+1);
		String fxnsubstr2 = fxn.substring(i, i+4+1);// 4 letter functions

		if (fxn.charAt(i) == ' ' || fxn.charAt(i) == ',') {continue;} // unwanted tokens or delimitter

		else if (isOperator(fxn.charAt(i))) {
			while (!CS.isCSEmpty() && CS.CStop() != '(' && ownHigherPrecedence(CS.CStop(), fxn.charAt(i))) {
				postfix += CS.CStop(); // higher precedence can be appended and push
				CS.CSpop();
			}
			CS.CSpush(fxn.charAt(i)); // now push the operator that we have just encounted to the stack
		}

		// operands are automatically append to the string since the order of operands from left to right won't change
		else if (isOperand(fxn.charAt(i))) {
			// search for numbers with multiple digits
			while (i < fxn.length() && isOperand(fxn.charAt(i))) {
				postfix += fxn.charAt(i);
				i++;
			}
			i--; // avoids overincrementing, further explanation in the postfixeval function
			postfix += ','; // allows the postfixeval fxn to better understand the character
		}

		// negative operands
		else if (isLeftBracket(fxn.charAt(i)) && fxn.charAt(i+1) == '-') {
			while (!isRightBracket(fxn.charAt(i))) {
				postfix += fxn.charAt(i);
				i++;
			}
			postfix += ')';
			postfix += ',';
		}

		/*------------------------------------------------------------------------------------*/
		else if (fxnsubstr == "tan(" || fxnsubstr == "sin(" || fxnsubstr == "cos(" || fxnsubstr == "exp(")
		{
			for (int j = 0; j <= 3; j++) {
				postfix += fxn.charAt(i);
				i++;
			}
			CharStack bracketcollector;
			bracketcollector.CSpush('(');
			String nestedfxn = "";
			while (!bracketcollector.isCSEmpty()) {
				if (isLeftBracket(fxn.charAt(i))) {
					bracketcollector.CSpush('(');
					nestedfxn += fxn.charAt(i);
					i++;
				}
				else if (isRightBracket(fxn.charAt(i))) {
					bracketcollector.CSpop();
					nestedfxn += fxn.charAt(i);
					i++;
				}
				else {
					nestedfxn += fxn.charAt(i);
					i++;
				}
			}
			i--;
			postfix.concat(infixtopostfix(nestedfxn)); // recursion and appension
			postfix += ')';
			postfix += ',';
		}
		/*------------------------------------------------------------------------------------*/

		/*------------------------------------------------------------------------------------*/
		else if (fxnsubstr2 == "atan(" || fxnsubstr2 == "acos(" || fxnsubstr2 == "asin("
					|| fxnsubstr2 == "tanh(" || fxnsubstr2 == "cosh(" || fxnsubstr2 == "sinh(")
		{
			for (int j = 0; j <= 4; j++) {
				postfix += fxn.charAt(i);
				i++;
			}
			CharStack bracketcollector;
			bracketcollector.CSpush('(');
			String nestedfxn = "";
			while (!bracketcollector.isCSEmpty()) {
				if (isLeftBracket(fxn.charAt(i))) {
					bracketcollector.CSpush('(');
					nestedfxn += fxn.charAt(i);
					i++;
				}
				else if (isRightBracket(fxn.charAt(i))) {
					bracketcollector.CSpop();
					nestedfxn += fxn.charAt(i);
					i++;
				}
				else {
					nestedfxn += fxn.charAt(i);
					i++;
				}
			}
			i--;
			postfix.concat(infixtopostfix(nestedfxn)); // recursion and appension
			postfix += ')';
			postfix += ',';

		}
		/*------------------------------------------------------------------------------------*/

		else if (isLeftBracket(fxn.charAt(i))) {
			CS.CSpush(fxn.charAt(i));
		}

		else if (isRightBracket(fxn.charAt(i))) {
			while (!CS.isCSEmpty() && !isLeftBracket(CS.CStop())) {
				postfix += CS.CStop();
				CS.CSpop();
			}
			CS.CSpop(); // one last extra pop to take care of the last opening parentheses
		}
	}
	// remaining operators that are still in stack can now be popped and placed on the expression
	while (!CS.isCSEmpty()) {
		postfix += CS.CStop();
		CS.CSpop();
	}

	return postfix;
}

// check if a character from the expression is a simple math operator
bool isOperator(char C) {
	if (C == '+' || C == '-' || C == '*' || C == '/' || C == '^') {return true;}
	return false;
}

// check if a charater from the expression is a NUMBER or is the variable in the function
bool isOperand(char C) {
	if ((C >= '0' && C <= '9') || C == '.' || C == 'z') {return true;}
	return false;
}

// used to check for weird chars in the flag function
bool isLeftBracket(char C) {
	if (C == '(') {return true;}
	return false;
}

// used to check for weird chars in the flag function
bool isRightBracket(char C) {
	if (C == ')') {return true;}
	return false;
}

// used to check for weird chars
bool flag(char C) {
	if (!isOperator(C) || !isOperand(C) || !isLeftBracket(C) || !isRightBracket(C)) {return true;}
	return false;
}

// perform simple math operator
double domath(char operation, double operand1, double operand2) {
	if (operation == '+') {return operand1 + operand2;}
	else if (operation == '-') {return operand1 - operand2;}
	else if (operation == '*') {return operand1 * operand2;}
	else if (operation == '/') {return operand1 / operand2;}
	else if (operation == '^') {return pow((float) operand1, (float) operand2);}
	else {Serial.print("Unknown error occured."); return -1;}
}

/*---special functions pertaining to the infixtopostfix conversion---*/

// used to verify if an operator is right associative
// (i.e., exponentiation is the arithmetic exception such that +,-,/,* ions are all left associative)
bool isRightAssociative(char operatorr) {
	if (operatorr == '^') {return true;}
	return false;
}

// used to get the "weight" of an operator such that higher weight means higher precedence
int getOperatorWeight (char operatorr) {
	int weight = -1;
	switch (operatorr) {
		case '-':
		case '+':
		weight = 1;
		break;

		case '/':
		case '*':
		weight = 2;
		break;

		case '^':
		weight = 3;
		break;
	}
	return weight;
}

// determines if the top of stack of operators has higher precedence (operatorr1)
bool ownHigherPrecedence(char operatorr1, char operatorr2) {
	int operatorr1weight = getOperatorWeight(operatorr1);
	int operatorr2weight = getOperatorWeight(operatorr2);

	// if the operators have equal precedence, return true is they are left associative
	// return false, if right associative
	// if operator is left-associative, the operator on the left is given higher priority
	if (operatorr1weight == operatorr2weight) {
		if(isRightAssociative(operatorr1)) {return false;}
		else {return true;}
	}
	return operatorr1 > operatorr2 ? true : false;
}

// very similar to class code by Paul and Chris
bool isBalanced(String fxn) {
	CharStack CS;
	for (unsigned int i = 0; i < fxn.length(); i++) {
		if (fxn.charAt(i) != '(' && fxn.charAt(i) != ')') { // this needs to be an AND check NOT an OR!
			continue;
		}
		else if (fxn.charAt(i) == '(') {
			CS.CSpush(fxn.charAt(i));
		}
		else {
			if (CS.isCSEmpty()) {
				return false;
			}
			if (fxn.charAt(i) == ')' && CS.CStop() != '(') {
				return false;
			}
			CS.CSpop();
		}
	}
	return CS.isCSEmpty();
}
