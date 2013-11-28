/*
 Dynamixel Basic Position Control Example
 
 Turns left the dynamixel , then turn right for one second,
 repeatedly.
 Dynamixel constants and control table are declared in below header
 work\hardware\robotis\cores\robotis\dxl_constants.h
 
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

//Dxl.writeWord(Dynamixel_Motor_Number, Address_Number, Address_Data);

void setup() {
  // Initialize the dynamixel bus:
  // baud rate index n=1 is 1Mbps refer to 2000000/(1+n)
  // 34 is 57600 bps
  Dxl.begin(1);  //1Mbps
}

void loop() {  
  Dxl.writeWord(1, 30, 0); //Turn dynamixel ID 1 to position 0
  delay(1000);              // Wait for 1 second (1000 milliseconds)
  Dxl.writeWord(1, 30, 300);//Turn dynamixel ID 1 to position 300
  delay(1000);              // Wait for 1 second (1000 milliseconds)
}

