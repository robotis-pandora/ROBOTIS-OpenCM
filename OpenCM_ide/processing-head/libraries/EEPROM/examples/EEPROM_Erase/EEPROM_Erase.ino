/*
  EEPROM Emulation Example - Erase -
 
 Demonstrates how to use flash memory as EEPROM in CM9 board.
 
 ************************* CAUTION *****************************
 New bootoader is needed to operate EEPROM library example.
 Because old bootloader always erase after flash address 0x08003000
 So, the data saved will be deleted after downloading new codes to cm9 board
 Please check the bootloader codes in gitHub and robotsource.org
 ***************************************************************
 Created by ROBOTIS,.LTD. 2013-06-12
 */

#include "EEPROM.h"
int i;
EEPROM CM9_EEPROM;
void setup(){  
  CM9_EEPROM.begin();
  //erase page 0, 1
  //instead of eraseAll(), -> CM9_EEPROM.erase(0), CM9_EEPROM.erase(1)
  CM9_EEPROM.eraseAll();
  //CM9_EEPROM.erase(0); 
  //CM9_EEPROM.erase(1);
}
void loop(){
  //after erasing data
  //just blinking LED.
  toggleLED();
  delay(100);
}

