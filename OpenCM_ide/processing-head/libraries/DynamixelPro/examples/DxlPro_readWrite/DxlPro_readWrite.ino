/*
  Dynamixel read/write exmaples

  Reads an dynaixel current movement, and set goal position
  turn left and right repeatly.
  
  You can also find all information about DYNAMIXEL PRO and protocol 2.0
  http://support.robotis.com/

  24 Jun. 2013  ported to CM9  by ROBOTIS,.LTD.
*/

#include <DynamixelPro.h>
DynamixelPro DxlEx(3);//DXL PRO(485) on Serial3(USART3)

void setup() {
  DxlEx.begin(1);// Baud num 1 is 57600 bps in DXL 2.0 control table
  //Toque is enable to move dynamixel pro
  DxlEx.writeByte(1,562,1);
}
byte isMoving = 0;
int goalPosition = 0;
void loop() {

  //Check if ID 1 dynamixel is still moving, address 610
  //Please refer ROBOTIS support page 
  //Check if the last communication is successful
  if(!DxlEx.readByte( 1,610)){
    
     //Send instruction packet to move for goalPosition( control address is 596 )
    DxlEx.writeDword( 1,596, goalPosition );
     //toggle the position
    if(goalPosition == 151875)
      goalPosition = -151875;
    else
      goalPosition = 151875;
    
  }  
  delay(100);
}


