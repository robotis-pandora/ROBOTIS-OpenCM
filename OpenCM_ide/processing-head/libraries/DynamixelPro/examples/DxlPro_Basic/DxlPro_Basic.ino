/*
  Dynamixel Basic exmaples

  Write 2 goal positions to ID 1 dynamixel pro
  turn left and right repeatly.
  Dynamixel pro use DXL protocol 2.0
  You can also find all information about DYNAMIXEL PRO and protocol 2.0
  http://support.robotis.com/

  24 Jun. 2013  ported to CM9  by ROBOTIS,.LTD.
*/
#include <DynamixelPro.h>

DynamixelPro DxlEx(3);//DXL PRO(485) on Serial3(USART3)

void setup() {
  DxlEx.begin(1);// Baud num 1 is 57600 bps in DXL 2.0 control table
  
  //Toque on to move dynamixel pro
  DxlEx.writeByte(1,562,1);
}

void loop() {

  //Turn on green LED in DXL PRO
  DxlEx.writeByte(1,564,255);
  //Move to goal position 151875 refer to position limit
  DxlEx.writeDword(1,596,151875);
  delay(2000);
  //Turn off green LED in DXL PRO
  DxlEx.writeByte(1,564,0);
  //Move to goal position -151875 refer to position limit
  DxlEx.writeDword(1,596,-151875);

  delay(2000);
  //Read DXL internal temperature
  SerialUSB.print(" DXL PRO Temperature = ");
  SerialUSB.println(DxlEx.readByte(1,625));

}


