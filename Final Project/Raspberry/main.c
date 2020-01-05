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
SoftwareSerial lcdSerial(2, 3);
LCDLib lcdControl;

int x, y, z;
int distanceI, distanceD, temperatureI, temperatureD, humidityI, humidityD;
char msg[100];

static WORKING_AREA(waThread_ARDUINO, 128);
static msg_t Thread_ARDUINO(void *p)
{
  (void)p;
  chRegSetThreadName("Thread_ARDUINO");
  uint8_t request = SENSORS;
  uint8_t result[12];
  msg_t status;

  // Some time to allow slaves initialization
  chThdSleepMilliseconds(2000);

  while (TRUE)
  {
    chBSemWait(&smph);

    // Request values
    i2cMasterTransmitTimeout(
        &I2C0, slave_address, request, 1,
        &result, 12, MS2ST(1000));
    chThdSleepMilliseconds(10);


    distanceI = (((int)result[1]) << 8) | result[0];
    distanceD = (((int)result[3]) << 8) | result[2];
    temperatureI = (((int)result[5]) << 8) | result[4];
    temperatureD = (((int)result[6]) << 8) | result[7];
    humidityI = (((int)result[8]) << 8) | result[9];
    humidityD = (((int)result[10]) << 8) | result[11];

    chThdSleepMilliseconds(1000);
    chBSemSignal(&smph);
  }
  return 0;
}

static WORKING_AREA(waThread_ADXL, 128);
static msg_t Thread_ADXL(void *p)
{
  (void)p;
  chRegSetThreadName("Thread_ADXL");

  uint8_t result[] = {0, 0, 0, 0, 0, 0};

  while (TRUE)
  {
    chBSemWait(&smph);
    i2cMasterTransmitTimeout(
        &I2C0, DEVICE_ADDRESS, DATAX0, 1,
        &result, 6, MS2ST(1000));

    // chThdSleepMilliseconds(1000);
    x = (((int)result[1]) << 8) | result[0];
    y = (((int)result[3]) << 8) | result[2];
    z = (((int)result[5]) << 8) | result[4];

    chBSemSignal(&smph);
  }
  return 0;
}

static WORKING_AREA(waThread_LCD, 128);
static msg_t Thread_LCD(void *p)
{
  (void)p;
  chRegSetThreadName("Thread_LCD");
  uint16_t iteration = 0;
  while (TRUE)
  {
    chBSemWait(&smph);

    lcdControl.eraseScreen();
    printDistance();
    printTemperatureAndHumidity();
    printAccelerometer();

    chThdSleepMilliseconds(1000);
    chBSemSignal(&smph);
  }
  return 0;
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
  //initializeLCD();
  initializeLCD2();
  initializeAccelerometer();

  chBSemInit(&smph, 0);

  // Start threads
  chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), HIGHPRIO, Thread_LCD, NULL);
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
  char txbuf[] = "Hello Chibi-World";

  // Initialize Serial Port
  sdStart(&SD1, NULL);

  // First Message
  chprintf((BaseSequentialStream *)&SD1, "Main (SD1 started)");

  // Coordinates
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x18);
  sdPut(&SD1, (uint8_t)0x00);
  chThdSleepMilliseconds(10);

  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x19);
  sdPut(&SD1, (uint8_t)0x0A);
  chThdSleepMilliseconds(10);

  // Second message
  chprintf((BaseSequentialStream *)&SD1, txbuf);
}

void initializeLCD2(){
  lcdControl.begin(&lcdSerial);   
  lcdControl.eraseScreen(); 
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

initializeAccelerometer()
{

  uint8_t request[] = {DATA_FORMAT, 0x01, POWER_CTL, 0x08};
  uint8_t result = 0;

  i2cMasterTransmitTimeout(
      &I2C0, DEVICE_ADDRESS, request, 4,
      &result, 0, MS2ST(1000));
}

void printDistance()
{
    sprintf(msg, "Distance: %i.%icm", distanceI, distanceD);
    lcdControl.print(0, 63, msg);
}

void printTemperatureAndHumidity()
{
    sprintf(msg, "Temperature: %i.%ideg C", temperatureI, temperatureD);
    lcdControl.print(0, 55, msg);

    sprintf(msg, "Humidity: %i.%i percent", humidityI, humidityD);
    lcdControl.print(0, 47, msg);
}

void printAccelerometer()
{
    sprintf(msg, "X: %i", x);
    lcdControl.print(0, 39, msg);

    sprintf(msg, "Y: %i", y);
    lcdControl.print(0, 31, msg);

    sprintf(msg, "Z: %i", z);
    lcdControl.print(0, 23, msg);
}
