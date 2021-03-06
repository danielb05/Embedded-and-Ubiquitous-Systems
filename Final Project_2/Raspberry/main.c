/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.
    This file is part of ChibiOS/RT.
    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.
    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "constants.h"

BSEMAPHORE_DECL(smph,0);

int x, y, z;
char distance[4] = {0}, temperature[4] = {0}, humidity[4] = {0};
uint8_t received[10] = {0};

void printAccelerometer(void);
void printTemperatureAndHumidity(void);
void printDistance(void);
void initializeAccelerometer(void);
void initializeI2C(void);
void initializeLCD(void);

static WORKING_AREA(waThread_ARDUINO, 256);
static msg_t Thread_ARDUINO(void *p)
{
  (void)p;
  chRegSetThreadName("Thread_ARDUINO");
  uint8_t request = SENSORS;

  // Some time to allow slaves initialization
  chThdSleepMilliseconds(1000);

  while (TRUE)
  {
    i2cAcquireBus(&I2C0);

    // Request values
    i2cMasterTransmit(
        &I2C0, slave_address, request, 1,
        &received, 9);
        
	i2cReleaseBus(&I2C0);
    chThdSleepMilliseconds(10);

    distance[0] = received[0];
    distance[1] = received[1];
    distance[2] = received[2];
    //distance[3] = 0;
    temperature[0] = received[3];
    temperature[1] = received[4];
    temperature[2] = received[5];
    //temperature[3] = 0;
    humidity[0] = received[6];
    humidity[1] = received[7];
    humidity[2] = received[8];
    //humidity[3] = 0;


    chThdSleepMilliseconds(3000);
  }
  return 0;
}

static WORKING_AREA(waThread_ADXL, 256);
static msg_t Thread_ADXL(void *p)
{
  (void)p;
  chRegSetThreadName("Thread_ADXL");
  
  uint8_t result[] = {0, 0, 0, 0, 0, 0};
  int data = 0x32;
  chThdSleepMilliseconds(1000);

  while (TRUE)
  {
	  i2cAcquireBus(&I2C0);

    i2cMasterTransmit(
		&I2C0, DEVICE_ADDRESS, &data, 1,
        result, 6);
		
	i2cReleaseBus(&I2C0);
    /*
    chprintf((BaseSequentialStream *)&SD1, "A: %d ", result[0]);
        chThdSleepMilliseconds(10);
    chprintf((BaseSequentialStream *)&SD1, "B: %d ", result[1]);
        chThdSleepMilliseconds(10);
    chprintf((BaseSequentialStream *)&SD1, "C: %d ", result[2]);
        chThdSleepMilliseconds(10);
    chprintf((BaseSequentialStream *)&SD1, "D: %d ", result[3]);
        chThdSleepMilliseconds(10);
    chprintf((BaseSequentialStream *)&SD1, "E: %d ", result[4]);
        chThdSleepMilliseconds(10);
    chprintf((BaseSequentialStream *)&SD1, "F: %d ", result[5]);
        chThdSleepMilliseconds(10);
        */
    x = (((int)result[1]) << 8) | result[0];
    y = (((int)result[3]) << 8) | result[2];
    z = (((int)result[5]) << 8) | result[4];

	chThdSleepMilliseconds(3000);    
  }
  return 0;
}

static WORKING_AREA(waThread_LCD, 256);
static msg_t Thread_LCD(void *p)
{
  (void)p;
  chRegSetThreadName("Thread_LCD");

  while (TRUE)
  {
	  
    printDistance();
    printTemperatureAndHumidity();
    printAccelerometer();

    chThdSleepMilliseconds(5000);
  }
  return 0;
}

void setLCDCoordinates(uint8_t x,  uint8_t y)
{
    // set X
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x18);
     //sdPut(&SD1, (uint8_t)0x38);
    sdPut(&SD1, x);
    chThdSleepMilliseconds(10);
	
    // set Y    
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x19);
    // sdPut(&SD1, (uint8_t)0x38);
    sdPut(&SD1, y);
    chThdSleepMilliseconds(10);
}
/*
 * Application entry point.
 */
int main(void)
{
  halInit();
  chSysInit();

  initializeI2C();
  //Don't know if the next function will be necessary, so I've commented it out
  initializeLCD();
  initializeAccelerometer();

  chBSemInit(&smph, 0);

  // Start threads
  chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), NORMALPRIO, Thread_LCD, NULL);
  chThdCreateStatic(waThread_ARDUINO, sizeof(waThread_ARDUINO), NORMALPRIO, Thread_ARDUINO, NULL);
  chThdCreateStatic(waThread_ADXL, sizeof(waThread_ADXL), NORMALPRIO, Thread_ADXL, NULL);

  /*
   * Events servicing loop.
   */
  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}

void initializeLCD()
{

  // Initialize Serial Port
  sdStart(&SD1, NULL);

  // First Message
  chprintf((BaseSequentialStream *)&SD1, "Final Project:");
}

void initializeI2C()
{
  // Initialize Serial Port
  sdStart(&SD1, NULL);

  /*
   * I2C initialization.
   */
  I2CConfig i2cConfig;
  i2cStart(&I2C0, &i2cConfig);
}

void initializeAccelerometer()
{

  uint8_t request[2] = {0x2D, 0x00};
  uint8_t result = 0;

  i2cMasterTransmit(
      &I2C0, DEVICE_ADDRESS, request, 2,
      &result, 0);
      
    chThdSleepMilliseconds(10);
      
	request[1] = 0x10;
  i2cMasterTransmit(
      &I2C0, DEVICE_ADDRESS, request, 2,
      &result, 0);

	chThdSleepMilliseconds(10);
    request[1] = 0x08;
  i2cMasterTransmit(
      &I2C0, DEVICE_ADDRESS, request, 2,
      &result, 0);

	chThdSleepMilliseconds(10);
   	request[0] = 0x31;
    request[1] = 0x01;
  i2cMasterTransmit(
      &I2C0, DEVICE_ADDRESS, request, 2,
      &result, 0);
    chThdSleepMilliseconds(10);
    
}

void printDistance()
{
    
    setLCDCoordinates((uint8_t)0, (uint8_t)56);
        chThdSleepMilliseconds(10);
    chprintf((BaseSequentialStream *)&SD1, "Distance: %s cm", distance);
        chThdSleepMilliseconds(10);
}

void printTemperatureAndHumidity()
{

    setLCDCoordinates((uint8_t)0, (uint8_t)48);
        chThdSleepMilliseconds(10);
    chprintf((BaseSequentialStream *)&SD1, "Temperature: %s deg C", temperature);
        chThdSleepMilliseconds(10);
        
    setLCDCoordinates((uint8_t)0, (uint8_t)40);
        chThdSleepMilliseconds(10);
	chprintf((BaseSequentialStream *)&SD1, "Humidity: %s percent", humidity);
	    chThdSleepMilliseconds(10);
}

void printAccelerometer()
{

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
