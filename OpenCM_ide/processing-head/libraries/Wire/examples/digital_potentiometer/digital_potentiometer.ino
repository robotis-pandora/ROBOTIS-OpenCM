// I2C Digital Potentiometer
// by Nicholas Zambetti <http://www.zambetti.com>
// and Shawn Bonkowski <http://people.interaction-ivrea.it/s.bonkowski/>
// Updated by Martin Mason for use with CM-900  <http://www.profmason.com>

// Demonstrates use of the Wire library
// Controls AD5171 digital potentiometer via I2C

// Created 31 March 2006

// This example code is in the public domain.



#include <Wire.h>

void setup()
{
  Wire.begin(14,15); // join i2c bus (SDA 14 and SCL 15)
}

byte val = 0;

void loop()
{
  Wire.beginTransmission(44); // transmit to device #44 (0x2c)
                              // device address is specified in datasheet
  Wire.write(byte(0x00));            // sends instruction byte  
  Wire.write(val);             // sends potentiometer value byte  
  Wire.endTransmission();     // stop transmitting

  val++;        // increment value
  if(val == 64) // if reached 64th position (max)
  {
    val = 0;    // start over from lowest value
  }
  delayMicroseconds(500);
}

