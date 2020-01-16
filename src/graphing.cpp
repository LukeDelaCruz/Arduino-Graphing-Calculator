//Course: CMPUT 274 Fa17

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <math.h>
#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 239
#define DWIDEMID ((DISPLAY_WIDTH)/2)-1
#define DHEIGHTMID ((DISPLAY_HEIGHT)/2)-1
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"stack.exercise.8.h"
#include"postfixeval.h"

double xvals[DISPLAY_WIDTH + 1] = {0}; // display width pixels
double yvals[DISPLAY_WIDTH + 1] = {0}; // extra entry for safety
double length, height, lowerbound, higherbound;
String xmininput, ymininput, xmaxinput, ymaxinput;
double xmininputnum, ymininputnum, xmaxinputnum, ymaxinputnum;
String function, menuselection;
void defaultinquiry();

void setup() {
	init(); Serial.begin(9600); tft.begin(); tft.setRotation(3); tft.fillScreen(ILI9341_BLACK);
}

// absolute value function that was modified to use doubles
double absmod(double x) {
	if (x < 0) {
		x = -x;
		return x;
	}
	else {
		return x;
	}
}

// gets x bounds and sets up the y axis
void domainSet(double leftbound, double rightbound, double xscl) {
	length = absmod(rightbound - leftbound);
	double increment = length/(DISPLAY_WIDTH+1);
	xvals[0] = leftbound;
	xvals[320] = rightbound;
	for (int i = 1; i < DISPLAY_WIDTH; i++) {
		xvals[i] = xvals[i-1] + increment;
	}
	tft.drawFastVLine((DISPLAY_WIDTH/length)*(-1)*(leftbound), 0, DISPLAY_HEIGHT+1, 0xFFFF); // y-axis as x=0
}

// gets y bounds and sets up the x axis
void rangeSet(double ymin, double ymax, double yscl) {
	lowerbound = ymin;
	higherbound = ymax;
	height = absmod(higherbound - lowerbound);
	tft.drawFastHLine(0, (DISPLAY_HEIGHT/height)*higherbound, DISPLAY_WIDTH, 0xFFFF); // x-axis as y = 0
}

// main grapher
// O(n) since it's a simple for loop
void grapher() {
	for (int i = 0; i < DISPLAY_WIDTH + 1; i++) {

		if (
			// excludes data that exceeds screen limits
				((((DISPLAY_HEIGHT+1)/height)*(yvals[i])  + ((DISPLAY_HEIGHT+1)/height)*(higherbound)) < 0
		&&  (((DISPLAY_HEIGHT+1)/height)*(yvals[i+1]) + ((DISPLAY_HEIGHT+1)/height)*(higherbound)) < 0)

		||

		    ((((DISPLAY_HEIGHT+1)/height)*(yvals[i])  + ((DISPLAY_HEIGHT+1)/height)*(higherbound)) > 239
		&&  (((DISPLAY_HEIGHT+1)/height)*(yvals[i+1]) + ((DISPLAY_HEIGHT+1)/height)*(higherbound)) > 239)

		|| 		// handles "obvious" vertical asymptotes (jump discontinuous from top then bottom) (e.g.,tan(pi/2))

				((((DISPLAY_HEIGHT+1)/height)*(yvals[i])  + ((DISPLAY_HEIGHT+1)/height)*(higherbound)) < 0
		&&  (((DISPLAY_HEIGHT+1)/height)*(yvals[i+1]) + ((DISPLAY_HEIGHT+1)/height)*(higherbound)) > 239)

		||

				((((DISPLAY_HEIGHT+1)/height)*(yvals[i])  + ((DISPLAY_HEIGHT+1)/height)*(higherbound)) > 239
		&&  (((DISPLAY_HEIGHT+1)/height)*(yvals[i+1]) + ((DISPLAY_HEIGHT+1)/height)*(higherbound)) < 0)
		||

			isinf(yvals[i]) == 1
		)
		{continue;}

		// uses point-to-point line connections for curve smoothness optimizations
		// uses (pixel/unit)*unit scaling for window changes)
		tft.drawLine(i,   ((DISPLAY_HEIGHT+1)/height)*(yvals[i])   + ((DISPLAY_HEIGHT+1)/height)*(higherbound),
						     i+1, ((DISPLAY_HEIGHT+1)/height)*(yvals[i+1]) + ((DISPLAY_HEIGHT+1)/height)*(higherbound),
								 0x26C2);
	}
}

// passes the user data to the functions that handle the rest
void funcvaluecalc(String input) {
	String variable = "z"; // desired variable INCLUDING BRACKETS
	String original = infixtopostfix(input);
	String infixed = infixtopostfix(input); // speed optimization
	char storageforxvals[50] = {0}; // buffer space for string input
	for (int i = 0; i < DISPLAY_WIDTH + 1; i++) {
		dtostrf(xvals[i], 1, 3, storageforxvals); // helps to convert doubles to string
		String xvalstring = String(storageforxvals);
		xvalstring = String("(" + xvalstring + ")"); // allows parser to recognize input nicer
		infixed.replace(variable, xvalstring); // modifies the fills in the values
		yvals[i] = (-1)*(postfixeval(infixed));
		infixed = original; // resets the infixed string to its original form
	}
	grapher();
}

// grabs user functions
void functioninquiry() {
	Serial.print("Enter y(z) = ");
	while (true) {
		if (Serial.available() > 0) {
			char c = Serial.read();
			if ((int) c == 13) {
				if (!isBalanced(function)) {
					Serial.println("Brackets are not balanced in your input. Please try again.");
					function = "";
					functioninquiry();
					break;
				}
				unsigned long initial = millis();
				funcvaluecalc(function);
				Serial.print("graphing..."); Serial.println("done!");
				Serial.print("Time taken: "); Serial.print((double)((millis() - initial) / 1000)); Serial.println("s"); Serial.println();
				function = ""; // reset this global string for reuse
				break;
			}
			else if ((int) c == 8) {
				function = function.substring(0, function.length()-1);
			}
			else{
				function += c;
			}
		}
	}
	Serial.flush();
}

// temporary attention diverter
void messagedisp() {
	tft.fillScreen(0);
  tft.setCursor(0, 0); // where the characters will be displayed
  tft.setTextSize(7);
  tft.setTextWrap(true);
	tft.print("View");
	tft.print("\n");
	tft.print("your");
	tft.print("\n");
	tft.print("serial monitor");
}

// sets up the user to arduino interaction
void defaultinquiry() {

	Serial.print("Domain is from (hit enter): ");
	while (true) {
		if (Serial.available() > 0) {
			char c = Serial.read();
			if ((int) c == 13) {
				xmininput = String("(" + xmininput + ")");
				xmininputnum = postfixeval(infixtopostfix(xmininput));
				break;
			}
			else if ((int) c == 8) {
				xmininput = xmininput.substring(0, xmininput.length()-1);
			}
			else{
				xmininput += c;
			}
		}
	}

	Serial.print("To: ");
	while (true) {
		if (Serial.available() > 0) {
			char c = Serial.read();
			if ((int) c == 13) {
				xmaxinput = String("(" + xmaxinput + ")");
				xmaxinputnum = postfixeval(infixtopostfix(xmaxinput));
				break;
			}
			else if ((int) c == 8) {
				xmaxinput = xmaxinput.substring(0, xmaxinput.length()-1);
			}
			else{
				xmaxinput += c;
			}
		}
	}

	Serial.print("Range is from (hit enter): ");
	while (true) {
		if (Serial.available() > 0) {
			char c = Serial.read();
			if ((int) c == 13) {
				ymininput = String("(" + ymininput + ")");
				ymininputnum = postfixeval(infixtopostfix(ymininput));
				break;
			}
			else if ((int) c == 8) {
				ymininput = ymininput.substring(0, ymininput.length()-1);
			}
			else{
				ymininput += c;
			}
		}
	}

	Serial.print("To: ");
	while (true) {
		if (Serial.available() > 0) {
			char c = Serial.read();
			if ((int) c == 13) {
				ymaxinput = String("(" + ymaxinput + ")");
				ymaxinputnum = postfixeval(infixtopostfix(ymaxinput));
				break;
			}
			else if ((int) c == 8) {
				ymaxinput = ymaxinput.substring(0, ymaxinput.length()-1);
			}
			else{
				ymaxinput += c;
			}
		}
	}
	tft.fillScreen(ILI9341_BLACK);

	// set the window view bounds after user input
	domainSet(xmininputnum, xmaxinputnum, 5);
	rangeSet (ymininputnum, ymaxinputnum, 5);

	Serial.println();

	while (menuselection != "end") {
		Serial.println("Choose: ");
		Serial.println("1: Graph a single function");
		Serial.println("2: Initialize graphing multiple functions on the same plane");
		Serial.println("3: End");
		Serial.print("Selection: ");
		while (true){
			if (Serial.available() > 0) {
				char c = Serial.read();
				if ((int) c == 13) {
					if (menuselection == "1") {
						Serial.println();
						Serial.println();
						functioninquiry();
						Serial.print("Program terminated. Re-enter serial monitor to restart.");
						menuselection = "end";
					}
					else if (menuselection == "2") {
						Serial.println();
						Serial.println();
						functioninquiry();
						menuselection = ""; // so we can reuse the global variable
						break;
					}
					else if (menuselection == "3") {
						Serial.println();
						Serial.println();
						Serial.print("Program terminated. Re-enter serial monitor to restart.");
						menuselection = "end";
					}
					else {
						Serial.println();
						Serial.println();
						Serial.println("Menu selection not recognized. Please try again");
						menuselection = "";
						break;
					}
				}
				else if ((int) c == 8) {
					menuselection = menuselection.substring(0, menuselection.length()-1);
				}
				else {
					menuselection += c;
				}
			}
		}
	}
}

int main() {
	setup();

	messagedisp();

	for (int i = 0; i < 70; i++) {Serial.println();} // clear screen
	Serial.println("Welcome to GRAPHING CALCULATOR via Arduino");
	Serial.println("Use local echo (ctrl-a + z + e) to view your input!");

	defaultinquiry();

	Serial.end();
	Serial.flush();
	return 0;
}
