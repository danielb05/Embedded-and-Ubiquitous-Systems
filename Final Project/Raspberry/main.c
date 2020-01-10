/*
			University of Lleida
	Master Of Science in Computer Science
		Embedded And Ubiquitous Systems
				  Professor
		  Fernando Guirado Fernández
				Final Project
				  Students
			Antonio Expósito Solis
			Daniel Vieira Cordeiro
			Rafael Câmara Pereira
*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ch.h"
#include "chvt.h"
#include "hal.h"
#include "chprintf.h"

// Device address
#define DEVICE_ADDRESS 0x53

// Slave address
#define slave_address 0x04

// Requests
#define SENSORS 1

// Global variables
int x, y, z;
char distance[4] = {0, 0, 0, 0};
char temperature[4] = {0, 0, 0, 0};
char humidity[4] = {0, 0, 0, 0};
uint8_t received[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Function prototypes
void printAccelerometer(void);
void printTemperatureAndHumidity(void);
void printDistance(void);
void initializeAccelerometer(void);
void initializeI2C(void);
void initializeLCD(void);

//		Thread that controls the requisitions to the arduino
// Set 256b of buffer memory to the thread
static WORKING_AREA(waThread_ARDUINO, 256);
static msg_t Thread_ARDUINO(void *p){

	(void)p;
	chRegSetThreadName("Thread_ARDUINO");
	uint8_t request = SENSORS;

	// Some time to allow slaves initialization
	chThdSleepMilliseconds(1000);

	while (TRUE){
		
		// Acquires the I2C bus exclusivey to the thread
		i2cAcquireBus(&I2C0);
		
		// Request values to the arduino
		i2cMasterTransmit(&I2C0, slave_address, request, 1, &received, 9);

		// Releases the I2C bus
		i2cReleaseBus(&I2C0);

		// Separates the bytes received between the three different data
		distance[0] = received[0];
		distance[1] = received[1];
		distance[2] = received[2];
		
		temperature[0] = received[3];
		temperature[1] = received[4];
		temperature[2] = received[5];
		
		humidity[0] = received[6];
		humidity[1] = received[7];
		humidity[2] = received[8];
		
		// Sleeps the thread to wait a time before creating a new request
		chThdSleepMilliseconds(1000);
	}

	return 0;
}


//		Thread that controls the requisitions to the accelerometer
// Set 256b of buffer memory to the thread
static WORKING_AREA(waThread_ADXL, 256);
static msg_t Thread_ADXL(void *p){

	(void)p;
	chRegSetThreadName("Thread_ADXL");

	// Sets some local variables
	// Result is the array to get the data from the accelerometer
	uint8_t result[] = {0, 0, 0, 0, 0, 0};
	// data is the position that gets X1 data from the accelerometer's register
	int data = 0x32;
	chThdSleepMilliseconds(1000);

	while (TRUE){

		// Acquires the I2C bus exclusivey to the thread
		i2cAcquireBus(&I2C0);

		// Request values to the accelerometer
		i2cMasterTransmit(&I2C0, DEVICE_ADDRESS, &data, 1, result, 6);

		// Releases the I2C bus
		i2cReleaseBus(&I2C0);

		// Calculates X, Y and Z for the accelerometer
		x = (((int)result[1]) << 8) | result[0];
		y = (((int)result[3]) << 8) | result[2];
		z = (((int)result[5]) << 8) | result[4];

		// Sleeps the thread to wait a time before creating a new request
		chThdSleepMilliseconds(1000);
	}

	return 0;
}

//		Thread that controls the LCD
// Set 256b of buffer memory to the thread
static WORKING_AREA(waThread_LCD, 256);
static msg_t Thread_LCD(void *p){

	(void)p;
	chRegSetThreadName("Thread_LCD");

	while (TRUE){

		// Prints the data received by the I2C
		printDistance();
		printTemperatureAndHumidity();
		printAccelerometer();

		// Sleeps the thread to wait a time before update the screen
		chThdSleepMilliseconds(5000);
	}
	
	return 0;
}

// Set the position of the screen to write the data
// x sets the starting pixel heigth
// y sets the ending pixel heigth
void setLCDCoordinates(uint8_t x,  uint8_t y){
	
    // set X
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x18);
    sdPut(&SD1, x);
    chThdSleepMilliseconds(10);
	
    // set Y    
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x19);
    sdPut(&SD1, y);
    chThdSleepMilliseconds(10);
}

// Initialization procedure to the LCD
void initializeLCD(){

	// Initialize Serial Port
	sdStart(&SD1, NULL);

	// First Message
	chprintf((BaseSequentialStream *)&SD1, "Final Project:");
}

// Initialization procedure to the I2C
void initializeI2C(){

	// I2C initialization.
	I2CConfig i2cConfig;
	i2cStart(&I2C0, &i2cConfig);
}

// Initialization procedure to the I2C
void initializeAccelerometer(){

	// Sets some local variables
	// Request is a two-byte array to initialyze the accelerometer setting some registers
	uint8_t request[2];
	uint8_t result = 0;

	// Command to reinitialize the accelerometer
	request[0] = 0x2D;
	request[1] = 0x00;
	i2cMasterTransmit(&I2C0, DEVICE_ADDRESS, request, 2, &result, 0);
	chThdSleepMilliseconds(10);

	// Command to set accelerometer to standby
	request[1] = 0x10;
	i2cMasterTransmit(&I2C0, DEVICE_ADDRESS, request, 2, &result, 0);
	chThdSleepMilliseconds(10);
	
	// Command to set accelerometer to measure
	request[1] = 0x08;
	i2cMasterTransmit(&I2C0, DEVICE_ADDRESS, request, 2, &result, 0);
	chThdSleepMilliseconds(10);
	
	// Command to set accelerometer to read the data
	request[0] = 0x31;
	request[1] = 0x01;
	i2cMasterTransmit(&I2C0, DEVICE_ADDRESS, request, 2, &result, 0);
	chThdSleepMilliseconds(10);
}

// Procedure to print the distance in centimeters
void printDistance(){

    setLCDCoordinates((uint8_t)0, (uint8_t)56);
	chThdSleepMilliseconds(10);
    
	chprintf((BaseSequentialStream *)&SD1, "Distance: %s cm", distance);
	chThdSleepMilliseconds(10);
}

// Procedure to print the Temperature in celsius degrees and humidity in percentage
void printTemperatureAndHumidity(){

    setLCDCoordinates((uint8_t)0, (uint8_t)48);
	chThdSleepMilliseconds(10);
    
	chprintf((BaseSequentialStream *)&SD1, "Temperature: %s deg C", temperature);
    chThdSleepMilliseconds(10);

    setLCDCoordinates((uint8_t)0, (uint8_t)40);
    chThdSleepMilliseconds(10);
	
	chprintf((BaseSequentialStream *)&SD1, "Humidity: %s percent", humidity);
	chThdSleepMilliseconds(10);
}

// Procedure to print the Accelerometer data
void printAccelerometer(){

	setLCDCoordinates((uint8_t)0, (uint8_t)32);
	chThdSleepMilliseconds(10);
	
	chprintf((BaseSequentialStream *)&SD1, "X: %05d", x);
	chThdSleepMilliseconds(10);

	setLCDCoordinates((uint8_t)0, (uint8_t)24);
	chThdSleepMilliseconds(10);
	
	chprintf((BaseSequentialStream *)&SD1, "Y: %05d", y);
	chThdSleepMilliseconds(10);

	setLCDCoordinates((uint8_t)0, (uint8_t)16);
	chThdSleepMilliseconds(10);
	
	chprintf((BaseSequentialStream *)&SD1, "Z: %05d", z);
	chThdSleepMilliseconds(10);
}

// Application entry point.
int main(void){

	// Initializes the Hardware Abstraction Layer driver
	halInit();
	
	// Initializes ChibiOS/RT, creating two threads
	chSysInit();

	// Initialize the components
	initializeI2C();
	initializeLCD();
	initializeAccelerometer();

	// Start threads
	chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), NORMALPRIO, Thread_LCD, NULL);
	chThdCreateStatic(waThread_ARDUINO, sizeof(waThread_ARDUINO), NORMALPRIO, Thread_ARDUINO, NULL);
	chThdCreateStatic(waThread_ADXL, sizeof(waThread_ADXL), NORMALPRIO, Thread_ADXL, NULL);

	// Blocks until finish
	chThdWait(chThdSelf());

	return 0;
}