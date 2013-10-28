// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>
// Updated by Martin Mason for use with CM-900  <http://www.profmason.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup()
{
  Wire.begin(14,15);        // join i2c bus (SDA 14 SCL 15)
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting

  x++;
  delayMicroseconds(500);
}
