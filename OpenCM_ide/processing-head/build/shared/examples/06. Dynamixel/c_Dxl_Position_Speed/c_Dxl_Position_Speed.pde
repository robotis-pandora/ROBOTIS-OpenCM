/*
 Dynamixel setPosition Example

 Turns left the dynamixel , then turn right for one second,
 repeatedly with different velocity.

 setPosition method is made by Martin S. Mason(Professor @Mt. San Antonio College)

 Created to CM9 series by ROBOTIS,.LTD.
*/
/*
*********************************************************************************
* Dynamixel instantiation on serial device
* To use dynamixel class, it must be needed for supporting OpenCM 485 SHD since v1.0.1
* Dynamixel bus can be operated on any serial device OpenCM board.
*********************************************************************************
*/
Dynamixel Dxl(1); //Dynamixel on Serial1(USART1)

void setup() {
  /*Initialize as 1Mbps baud rate*/
  Dxl.begin(1);
}

void loop() {
  /*ID 1 dynamixel moves to position 0 with velocity 100*/
  Dxl.setPosition(1,0,100);
  delay(1000);// it has more delay time for slow movement
  /*ID 1 dynamixel moves to position 500 with velocity 300*/
  Dxl.setPosition(1,500,300);
  delay(500);
}

