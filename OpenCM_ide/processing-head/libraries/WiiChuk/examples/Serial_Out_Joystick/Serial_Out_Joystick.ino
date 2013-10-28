/*
* Wii Nunchuck library example by Tim Hirzel http://www.growdown.com
* bugs fixed and wrapped in to standard library format
* Updated to support the CM-900 by Martin Mason http://www.profmason.com
*
* Be sure to pull up your two I2C ports with ~2K resistors to the 3.3V line.  
* By default using the pins 14 and 15 for SDA and SCL.
*/

#include "Wire.h"
#include "WiiChuk.h"

WiiChuck chuck = WiiChuck();

void setup() {
  SerialUSB.begin();
  chuck.begin(14,15);  //Specify SDA, SCL for this nunchuck
  chuck.update();
  chuck.calibrateJoy();
}

void loop() {
  delay(20);
  chuck.update();

  SerialUSB.print(chuck.readJoyX());
    SerialUSB.print(", ");  
  SerialUSB.print(chuck.readJoyY());
    SerialUSB.print(", ");  

  if (chuck.buttonZ) {
     SerialUSB.print("1");
  } else  {
     SerialUSB.print("0");
  }

    SerialUSB.print(", ");  

  if (chuck.buttonC) {
     SerialUSB.print("1");
  } else  {
     SerialUSB.print("0");
  }

    SerialUSB.println();

}
