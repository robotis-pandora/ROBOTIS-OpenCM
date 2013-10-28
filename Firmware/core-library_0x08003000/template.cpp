/* Copyright (C) 2013 ROBOTIS, Co., Ltd.
 *
 * @File: Template.cpp
 * @Brief : Template codes for ROBOTIS CM-9
 * 
 */


#include "Pandora.h"
/*
* This is a template for testing user codes 
* Write sketch code like codes in ROBOTIS CM-9
* After building in eclipse CDT and using ST-LINK you can download to cm-9 series boards
*/
void setup() {
  pinMode(BOARD_LED_PIN, OUTPUT);
}

void loop() {
  toggleLED();
  delay(100);
}
