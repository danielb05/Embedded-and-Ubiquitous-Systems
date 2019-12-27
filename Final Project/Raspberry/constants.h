#include "cstdlib"
#include "stdio.h"
#include "stdlib.h"
#include "string"


// Device address
// const int
#define DEVICE_ADDRESS = (0x53);  

// ADXL345 record addresses
// char
#define POWER_CTL = 0x2D;
#define DATA_FORMAT = 0x31;
#define DATAX0 = 0x32;   //X-Axis Data 0
#define DATAX1 = 0x33;   //X-Axis Data 1
#define DATAY0 = 0x34;   //Y-Axis Data 0
#define DATAY1 = 0x35;   //Y-Axis Data 1
#define DATAZ0 = 0x36;   //Z-Axis Data 0
#define DATAZ1 = 0x37;   //Z-Axis Data 1

// Slave address
// static const uint8_t
#define slave_address = 0x04;