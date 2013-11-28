/*
 Dynamixel ID Change Example
 
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
#define ID_Num 1

/* Address_Number_Define */
#define ID_Change_Address 3
#define Goal_Postion_Address 30
#define Moving_Speed 32

void setup() {
  // Initialize the dynamixel bus:
  Dxl.begin(1);  //1Mbps 
  /*************** CAUTION ***********************
  * All dynamixels in bus will be changed to ID 2 by using broadcast ID(0xFE)
  * Please check if there is only one dynamixel that you want to change ID
  ************************************************/
  Dxl.writeByte(0xFE, ID_Change_Address, 2);  //Dynamixel_Id_Number_Change 1 to 2
}

void loop() {
  //If the alternation was successful, the below code works well. 
  Dxl.writeWord(2, Goal_Postion_Address, 0); //Turn dynamixel ID 2 to position 0
  delay(1000);              // Wait for 1 second (1000 milliseconds)
  Dxl.writeWord(2, Goal_Postion_Address, 300);//Turn dynamixel ID 2 to position 300
  delay(1000);              // Wait for 1 second (1000 milliseconds)
}

