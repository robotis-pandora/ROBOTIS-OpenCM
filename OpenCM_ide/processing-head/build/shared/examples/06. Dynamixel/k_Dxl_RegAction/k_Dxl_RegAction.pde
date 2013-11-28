/*
 Dynamixel Reg Action Example
 
 This example shows how to use reg/action instruction using new packet methods
 Open serial monitor and type 'a' on keyboard
 you can check moving dynamixel, type 'a' again -> moves its origin postion 0
 
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
void setup(){
  Dxl.begin(1);
}
char temp;// need to receive from USB(PC)
uint32 GoalPos = 0;

void loop(){
  /*initPacket method needs ID and instruction*/
  Dxl.initPacket(1,INST_REG_WRITE);
  /* From now, insert byte data to packet without any index or data length*/
  Dxl.pushByte(30);
  Dxl.pushByte(DXL_LOBYTE(GoalPos));
  Dxl.pushByte(DXL_HIBYTE(GoalPos));
  /* just transfer packet to dxl bus without any arguments*/
  Dxl.flushPacket();
  if(Dxl.getResult()==(1<<COMM_RXSUCCESS) )
    SerialUSB.println("Comm Success");

  delay(100);
  if(SerialUSB.available()){
    temp = SerialUSB.read();
    if(temp == 'a'){
      SerialUSB.println("Reg Action");//Now start move dynamixel
      Dxl.initPacket(BROADCAST_ID,INST_ACTION);
      /* Action instruction does not need any push bytes*/
      Dxl.flushPacket();
      if( Dxl.getResult()==(1<<COMM_RXSUCCESS) ){
        SerialUSB.println("Comm Success");
        if(GoalPos == 0)
          GoalPos = 1023;
        else
          GoalPos = 0;
      }      
    }
  }
}

