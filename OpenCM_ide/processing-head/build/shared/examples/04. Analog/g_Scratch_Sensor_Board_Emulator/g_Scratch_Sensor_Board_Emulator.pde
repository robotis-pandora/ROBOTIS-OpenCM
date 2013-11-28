// Program Name: ArduinoBoard
// Version: 0.6 
// Author: Rai and Mike McKenzie
// Description: A program that simulates a Scratch board using 
//              an Arduino. This version reads the button and 
//              inputs A-D.
// 
//
// Based on Relay Wii Nunchuck status to Scratch
// By Mike Donovan
// ported to OpenCM Board By ROBOTIS,.LTD. 13 Sep 2013
//
// 2013-08-08 Ported by ROBOTIS CO.,LTD.
//============================================================================

//#include <Wire.h>
#include <OLLO.h>
OLLO myOLLO;

void ScratchBoardSensorReport(int sensor, int value)
{
  SerialUSB.print( B10000000
    | ((sensor & B1111)<<3)
    | ((value>>7) & B111),
  BYTE);
  SerialUSB.print( value & B1111111, BYTE);
}

void setup()
{
  //Serial.begin(38400);
  pinMode(BOARD_BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(BOARD_LED_PIN, OUTPUT);
  myOLLO.begin(1,TOUCH_SENSOR);
  myOLLO.begin(2, IR_SENSOR);
  myOLLO.begin(3);
  myOLLO.begin(4);

}
int irSensor = 0;
int gyroX=0;
int gyroY=0;
void loop() 
{ 

  // Format output for ScratchBoard emulation
  // sensor=0-7, value=0-1023 (rescaled by Scratch to 0-100)
  // 0="A", 1="B", 2="C", 3="D",
  // 4="Slider", 5="Light", 6="Sound", 7="Button"
  if(SerialUSB.available()){
    gyroX = myOLLO.read(3);
    if(gyroX > 1000 && gyroX < 2000){
      gyroX = 1530;
    }
    gyroY = myOLLO.read(4);
    if(gyroY > 1000 && gyroY < 2000){
      gyroY = 1530;
    }
    ScratchBoardSensorReport(0, map(gyroX, 0, 3000, 0, 1023) );
    ScratchBoardSensorReport(1, map(gyroY, 0, 3000, 0, 1023) );
    ScratchBoardSensorReport(2, 0/*analogRead(3)*/);
    ScratchBoardSensorReport(3, 0/*analogRead(4)*/);
    irSensor = myOLLO.read(2, IR_SENSOR);
    if(irSensor > 250)
      irSensor = 300;
    ScratchBoardSensorReport(4, map(irSensor, 0, 300, 0, 1023));
    ScratchBoardSensorReport(5, 0);
    ScratchBoardSensorReport(6, 0);
    ScratchBoardSensorReport(7, (myOLLO.read(1, TOUCH_SENSOR) ? 1023 : 0));
    // Let Scratch catch up with us
    delay(30);
    toggleLED();
  }
}









