/*
  Dynamixel SyncWrite exmaple using new packet methods
  This example shows same movement as previous syncwrite
  It does not need any complex length fomula and parameter index.
 
 You can buy DYNAMIXEL in ROBOTIS-SHOP
 [KOREAN]
 http://www.robotis-shop-kr.com/goods_list.php?Index=273
 [English]
 http://www.robotis-shop-en.com/shop/step_submain.php?b_code=B20070914050547
 
  You can also find all information about DYNAMIXEL
  http://support.robotis.com/  
  created 13 Sep. 2013  by ROBOTIS,.LTD.
*/
#define ID_NUM_1  1
#define ID_NUM_2  2
#define ID_NUM_3  3

#define P_GOAL_POSITION    30
#define P_GOAL_SPEED    32

/********* Sync write data  **************
 * ID1, DATA1, DATA2..., ID2, DATA1, DATA2,...
 ******************************************
 */
/*
 *********************************************************************************
* Dynamixel instantiation on serial device
* To use dynamixel class, it must be needed for supporting OpenCM 485 SHD since v1.0.1
* Dynamixel bus can be operated on any serial device OpenCM board.
*********************************************************************************
*/
Dynamixel Dxl(1); //Dynamixel on Serial1(USART1)

 
int SyncPage1[9]=
{ 
  ID_NUM_1,0,100,  // 3 Dynamixels are move to position 0
  ID_NUM_2,0,100,  // with velocity 100
  ID_NUM_3,0,100}; // 3 Dynamixels are move to position 512
int SyncPage2[9]=
{ 
  ID_NUM_1,512,500,// with velocity 500
  ID_NUM_2,512,500,
  ID_NUM_3,512,500};

void setup(){
  Dxl.begin(1);
  // Set goal speed
  Dxl.writeWord( BROADCAST_ID, P_GOAL_POSITION, 0 );
  // Set goal position
  Dxl.writeWord( BROADCAST_ID, P_GOAL_SPEED, 0 );
}
/*
* byte syncWrite(byte start_addr, byte num_of_data, int *param, int array_length);//new
*/
void loop(){
  Dxl.syncWrite(30,2,SyncPage1,9);
  delay(1000);
  Dxl.syncWrite(30,2,SyncPage2,9);
  delay(1000);
}


