/*

Antonio Expósito Solis
Daniel Vieira Cordeiro
Rafael Câmara Pereira

*/

#include "Arduino.h"
#include "lcd_lib.h"

// Constructor
LCDLib::LCDLib(){};
	        
// Defines the virtual port used with the Serial LCD
void LCDLib::begin(SoftwareSerial *sf_serial_port){
	virtualPort = sf_serial_port;
	virtualPort->begin(115200);
	eraseScreen();
};

// Clears all the screen
void LCDLib::eraseScreen(){
	virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(CLEAR_SCREEN);
}

// Clears the line and below
void LCDLib::eraseBox(int x){
	virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(CLEAR_BOX);
	virtualPort->write((byte)0);
	virtualPort->write((byte)0);
	virtualPort->write((byte)127);
	virtualPort->write((byte)x);
};

// Prints the str string at the (x,y) location
void LCDLib::print(int x, int y, char *str){
	virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(X_POSITION_SCREEN);
	virtualPort->write((byte)x);
	virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(Y_POSITION_SCREEN);
	virtualPort->write((byte)y);
	//virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(str);
};

// Set the pixel at coordinate (x,y)
void LCDLib::sPixel (int x, int y){
	virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(SET_PIXEL_SCREEN);
	virtualPort->write((byte)x);
	virtualPort->write((byte)y);
	virtualPort->write(ACTIVE);
};

// Clear the pixel at coordinate (x,y)
void LCDLib::clPixel (int x, int y){
	virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(SET_PIXEL_SCREEN);
	virtualPort->write((byte)x);
	virtualPort->write((byte)y);
	virtualPort->write(INACTIVE);
};

// Draws a line from (x1,y1) to (x2,y2) with ink value (0 clear, 1 set)
void LCDLib::line(int x1, int y1,  int x2, int y2, int ink){
	virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(DRAW_LINE_SCREEN);
	virtualPort->write((byte)x1);
	virtualPort->write((byte)y1);
	virtualPort->write((byte)x2);
	virtualPort->write((byte)y2);
	virtualPort->write((byte)ink);
};

// Draw a rectangle with ink value  with ink value (0 clear, 1 set)
void LCDLib::rectangle (int x, int y, int length, int height, int ink){
	virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(DRAW_RECTANGLE_SCREEN);
	virtualPort->write((byte)x);
	virtualPort->write((byte)y);
	virtualPort->write((byte)length);
	virtualPort->write((byte)height);
	//virtualPort->write((byte)ink);
};

// Draw a circle with center at (x,y) and radius
void LCDLib::circle (int x, int y, int radius, int ink){
	virtualPort->write(START_COMMAND_SCREEN);
	virtualPort->write(DRAW_CIRCLE_SCREEN);
	virtualPort->write((byte)x);
	virtualPort->write((byte)y);
	virtualPort->write((byte)radius);
	virtualPort->write((byte)ink);
};