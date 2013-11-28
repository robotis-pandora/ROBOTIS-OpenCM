/*
  Echo by USART3

  Demonstrates sending data from the computer to the CM900,
  echoes it to the computer again.
  You can just type in terminal program, character you typed will be displayed
  
  You can connect the below products to J9 Connector in CM-900
  [BT-110A] or [BT-110A Set]
  http://www.robotis-shop-kr.com/goods_detail.php?goodsIdx=875
  [ZIG-110A Set]
  http://www.robotis-shop-kr.com/goods_detail.php?goodsIdx=405
  [LN-101] download tool in CM-100
  http://www.robotis-shop-kr.com/goods_detail.php?goodsIdx=348
  
  You can also find all information about ROBOTIS products
  http://support.robotis.com/
  
  created 19 Dec 2012 by ROBOTIS,.LTD.
*/

/*
Serial1 : Dynamixel_Poart
Serial2 : Serial_Poart(4pin_Molex)
Serial3 : Serial_Poart(pin26:Tx3, pin27:Rx3)

TxD3(Cm9_Pin26) <--(Connect)--> RxD(PC)
RxD3(Cm9_Pin27) <--(Connect)--> TxD(PC)
*/

void setup(){
  //Serial2 Serial initialize
  Serial3.begin(57600);
  //You can attach your serial interrupt
  //or, also detach the interrupt by detachInterrupt(void) method
  Serial3.attachInterrupt(serialInterrupt);
  pinMode(BOARD_LED_PIN, OUTPUT);  //toggleLED_Pin_Out
}

//void interrupt_name (byte variable){}
void serialInterrupt(byte buffer){
  Serial3.print((char)buffer);
}

void loop(){
  toggleLED();
  delay(50);
}

