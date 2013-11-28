/*
  Dynamixel read/write exmaples

  Reads an dynaixel current position, and set goal position
  turn left and right repeatly.
  
  2013-04-10 removed some delay 1sec after ID 1 dxl changes its moving direction
  Actually it was not needed such 1 sec delay in previous version.
  Dynamixel constants and control table are declared in below header
  work\hardware\robotis\cores\robotis\dxl_constants.h

 You can buy DYNAMIXEL in ROBOTIS-SHOP
 [KOREAN]
 http://www.robotis-shop-kr.com/goods_list.php?Index=273
 [English]
 http://www.robotis-shop-en.com/shop/step_submain.php?b_code=B20070914050547
 
  You can also find all information about DYNAMIXEL
  http://support.robotis.com/

  created 14 Nov. 2012  ported to CM9  by ROBOTIS,.LTD.
*/

/*
*********************************************************************************
* Dynamixel instantiation on serial device
* To use dynamixel class, it must be needed for supporting OpenCM 485 SHD since v1.0.1
* Dynamixel bus can be operated on any serial device OpenCM board.
*********************************************************************************
*/
Dynamixel Dxl(1); //Dynamixel on Serial1(USART1)

byte isMoving = 0;
int goalPosition = 0;

void setup() {
  //Initialize dynamixel bus as 1Mbps baud rate
  Dxl.begin(1);  
}

void loop() {
  //Check if ID 1 dynamixel is still moving, 46 is moving control address
  //Please refer ROBOTIS support page
  isMoving = Dxl.readByte( 1, 46);
  //Check if the last communication is successful
  if( isMoving == 0 ){ //if ID 1 dynamixel is stopped
     //Send instruction packet to move for goalPosition( control address is 30 )
    Dxl.writeWord( 1, 30, goalPosition );
     //toggle the position if goalPosition is 1000, set to 0, if 0, set to 1000
    if(goalPosition == 1000)
      goalPosition = 0;
    else
      goalPosition = 1000;
  }

}
