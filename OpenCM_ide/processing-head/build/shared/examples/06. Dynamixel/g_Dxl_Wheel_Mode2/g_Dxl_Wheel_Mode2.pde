/*
 Dynamixel Wheel Mode 2 Example
 
 This example shows how to implement RC Car using two dynamixels
 The two dynamixels are set to as wheel mode by the way of changing
 CCW_Angle_Limit to 0
 If you want to use a dynamixel as joint mode, set CCW_Angle_Limit 
 to 0x3FF(1023) in AX-12 case
 
 You can buy DYNAMIXEL in ROBOTIS-SHOP
 [KOREAN]
 http://www.robotis-shop-kr.com/goods_list.php?Index=273
 [English]
 http://www.robotis-shop-en.com/shop/step_submain.php?b_code=B20070914050547
 
 You can also find all information about DYNAMIXEL
 http://support.robotis.com/
 
 Created by ROBOTIS,.LTD. 13 Sep 2013 
 */

//Dxl.writeWord(Dynamixel_Motor_Number, Address_Number, Address_Data);
#define ID_Left_Wheel 1
#define ID_Right_Wheel 2

/* Address_Number_Define */
#define CCW_Angle_Limit 8    //Wheel_Mode_Setting_Address_Number (Address_Data = 0, Wheel_Mode)
#define Goal_Postion 30
#define Moving_Speed 32
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
  Dxl.begin(1);  //1Mbps
  Dxl.writeByte(ID_Left_Wheel, CCW_Angle_Limit, 0);  //Dynamixel_Wheel_Mode
  Dxl.writeByte(ID_Right_Wheel, CCW_Angle_Limit, 0);  //Dynamixel_Wheel_Mode
}

void loop() {
  //
  Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 0); // stop at first
  Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0); 
  delay(1000);              // Wait for 1 second (1000 milliseconds)

  Dxl.writeWord(ID_Left_Wheel, Moving_Speed, 300);// go ahead with velocity 300
  Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0x3FF | 300);
  delay(2000);              // Wait for 2 second

  Dxl.writeWord(ID_Left_Wheel, Moving_Speed,  600); // speed up 
  Dxl.writeWord(ID_Right_Wheel, Moving_Speed, 0x3FF | 600);
  delay(2000);              // Wait for 2 second
}


