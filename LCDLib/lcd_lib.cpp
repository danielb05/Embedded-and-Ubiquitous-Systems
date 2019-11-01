/*

Antonio Expósito Solis
Daniel Vieira Cordeiro
Rafael Câmara Pereira

*/

#include "Arduino.h"
#include "LCDLib.h"

// Constructor
LCDLib::LCDLib(){};
	        
// Defines the virtual port used with the Serial LCD
LCDLib::begin(SoftwareSerial *sf_serial_port){
	
	Serial.begin(115200);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	
	sf_serial_port.begin(115200);
};

// Prints the str string at the (x,y) location
LCDLib::print(int x, int y, char *str){
	Serial.write(START_COMMAND_SCREEN);
	Serial.write(X_POSITION_SCREEN);
	Serial.write((byte)x);
	Serial.write(START_COMMAND_SCREEN);
	Serial.write(Y_POSITION_SCREEN);
	Serial.write((byte)y);
	Serial.write(str);
};

// Set the pixel at coordinate (x,y)
LCDLib::Pixel (int x, int y){
	Serial.write(START_COMMAND_SCREEN);
	Serial.write(SET_PIXEL_SCREEN);
	Serial.write((byte)x);
	Serial.write((byte)y);
	Serial.write(ACTIVE);
};

// Clear the pixel at coordinate (x,y)
LCDLib::clPixel (int x, int y){
	Serial.write(START_COMMAND_SCREEN);
	Serial.write(SET_PIXEL_SCREEN);
	Serial.write((byte)x);
	Serial.write((byte)y);
	Serial.write(INACTIVE);
};

// Draws a line from (x1,y1) to (x2,y2) with ink value (0 clear, 1 set)
LCDLib::line(int x1, int y1,  int x2, int y2, int ink){
	Serial.write(START_COMMAND_SCREEN);
	Serial.write(DRAW_LINE_SCREEN);
	Serial.write((byte)x1);
	Serial.write((byte)y1);
	Serial.write((byte)x2);
	Serial.write((byte)y2);
	Serial.write((byte)ink);
};

// Draw a rectangle with ink value  with ink value (0 clear, 1 set)
LCDLib::rectangle (int x, int y, int length, int height, int ink){
	Serial.write(START_COMMAND_SCREEN);
	Serial.write(DRAW_RECTANGLE_SCREEN);
	Serial.write((byte)x);
	Serial.write((byte)y);
	Serial.write((byte)length);
	Serial.write((byte)height);
	Serial.write((byte)ink);
};

// Draw a circle with center at (x,y) and radius
LCDLib::circle (int x, int y, int radius){
	Serial.write(START_COMMAND_SCREEN);
	Serial.write(DRAW_CIRCLE_SCREEN);
	Serial.write((byte)x);
	Serial.write((byte)y);
	Serial.write((byte)radius);
};