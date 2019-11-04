/*

Antonio Expósito Solis
Daniel Vieira Cordeiro
Rafael Câmara Pereira

*/

#ifndef LCDLib_H
#define LCDLib_H

#include "Arduino.h"
#include <SoftwareSerial.h>

class LCDLib{

    public:
        LCDLib();
		// Defines the virtual port used with the Serial LCD
        void begin(SoftwareSerial *sf_serial_port);
		// Prints the str string at the (x,y) location
        void print(int x, int y, char *str);		
        // Set the pixel at coordinate (x,y)
		void sPixel (int x, int y);
		// Clear the pixel at coordinate (x,y)
		void clPixel (int x, int y);
		// Draws a line from (x1,y1) to (x2,y2) with ink value (0 clear, 1 set)
		void line(int x1, int y1,  int x2, int y2, int ink);
		// Draw a rectangle with ink value  with ink value (0 clear, 1 set)
		void rectangle (int x, int y, int length, int height, int ink);
		// Draw a circle with center at (x,y) and radius 
		void circle (int x, int y, int radius);
		
		SoftwareSerial *virtualPort;
		
		byte START_COMMAND_SCREEN = 0x7C;
		byte CLEAR_SCREEN = 0x00;
		byte X_POSITION_SCREEN = 0x18;
		byte Y_POSITION_SCREEN = 0x19;
		byte SET_PIXEL_SCREEN = 0x10;
		byte DRAW_LINE_SCREEN = 0x0C;
		byte DRAW_RECTANGLE_SCREEN = 0x0F;
		byte DRAW_CIRCLE_SCREEN = 0x03;
		byte ACTIVE = 0x01;
		byte INACTIVE = 0x00;
};

#endif