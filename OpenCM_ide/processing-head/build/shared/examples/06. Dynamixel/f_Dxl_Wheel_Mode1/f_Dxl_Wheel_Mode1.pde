/*
 Dynamixel Wheel Mode Example
 
 This example shows how to use dynamixel as wheel mode
 All dynamixels are set as joint mode in factory,
 but if you want to make a wheel using dynamixel, 
 you have to change it to wheel mode by change CCW angle limit to 0
 
 You can buy DYNAMIXEL in ROBOTIS-SHOP
 [KOREAN]
 http://www.robotis-shop-kr.com/goods_list.php?Index=273
 [English]
 http://www.robotis-shop-en.com/shop/step_submain.php?b_code=B20070914050547
 
 You can also find all information about DYNAMIXEL
 http://support.robotis.com/
 
 Ported to CM9 from the ROBOTIS E-manual 21 Oct 2012
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
  // Initialize the dynamixel bus:
  Dxl.begin(1);
  Dxl.writeWord(1, 8, 0); //disable CCW Angle Limit(L) to use wheel mode
  //Dxl.writeWord(1, 8, 0x3FF); // use this code to revert to joint mode, please refer support.robotis.com
}

void loop() {
  Dxl.writeWord(1, 32, 400); //forward
  delay(5000);
  Dxl.writeWord(1, 32, 400 | 0x400); //reverse
  delay(5000); 
  Dxl.writeWord(1, 32, 0); //stop
  delay(2000);
}
