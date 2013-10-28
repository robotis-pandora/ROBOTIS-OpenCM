// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>
// Updated by Martin Mason for use with CM-900  <http://www.profmason.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup()
{
  Wire.begin(14,15);        // join i2c bus (SDA 14 SCL 15)
  SerialUSB.begin();  // start serial for output
}

void loop()
{
  Wire.requestFrom(2, 7);    // request 6 bytes from slave device #2

  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read(); // receive a byte as character
    SerialUSB.print(c);         // print the character
  }

  delayMicroseconds(500);
}
