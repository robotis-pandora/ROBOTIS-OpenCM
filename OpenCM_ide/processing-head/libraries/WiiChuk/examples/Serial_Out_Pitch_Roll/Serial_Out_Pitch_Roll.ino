/*
* Wii Nunchuck library example by Tim Hirzel http://www.growdown.com
* bugs fixed and wrapped in to standard library format
* Updated to support the CM-900 by Martin Mason http://www.profmason.com
*
* Be sure to pull up your two I2C ports with ~2K resistors to the 3.3V line.  
* By default using the pins 14 and 15 for SDA and SCL.
* 
*/


#include "Wire.h"
#include "WiiChuk.h"

#define MAXANGLE 90
#define MINANGLE -90


WiiChuck chuck = WiiChuck();
int angleStart, currentAngle;
int tillerStart = 0;
double angle;

void setup() {
  //nunchuck_init();
  SerialUSB.begin();
  chuck.begin(14,15);  //Specify SDA, SCL for this nunchuck
  chuck.update();
  //chuck.calibrateJoy();
}


void loop() {
  delay(20);
  chuck.update();


  SerialUSB.print(chuck.readRoll());
    SerialUSB.print(", ");  
  SerialUSB.print(chuck.readPitch());
    SerialUSB.print(", ");  

    SerialUSB.print((int)chuck.readAccelX());
    SerialUSB.print(", ");  
    SerialUSB.print((int)chuck.readAccelY());
    SerialUSB.print(", ");  

    SerialUSB.print((int)chuck.readAccelZ());

    SerialUSB.println();

}
