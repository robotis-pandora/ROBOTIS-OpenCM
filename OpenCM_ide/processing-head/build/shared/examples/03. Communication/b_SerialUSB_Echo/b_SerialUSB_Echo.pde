/*
 SerialUSB_Echo(Echo with USB Interrupt)
 
 Demonstrates sending data from the computer to the CM900,
 echoes it to the computer again.
 You can just type in terminal program, character you typed will be displayed
 
 
 created 19 Dec 2012
 by ROBOTIS,.LTD.
 
 */

void setup(){
  //USB Serial initialize
  SerialUSB.begin();
  //You can attach your serialUSB interrupt
  //or, also detach the interrupt by detachInterrupt(void) method
  SerialUSB.attachInterrupt(usbInterrupt);
  pinMode(BOARD_LED_PIN, OUTPUT);  //toggleLED_Pin_Out
}

//SerialUSB Interrupt type must have the below proto-type
//void interrupt_name (byte* buffer, byte nCount){}
//USB max packet data is maximum 64byte, so nCount can not exceeds 64 bytes
//int i=0;
void usbInterrupt(byte* buffer, byte nCount){
  SerialUSB.print("nCount =");
  SerialUSB.println(nCount);
  for(unsigned int i=0; i < nCount;i++)  //printf_SerialUSB_Buffer[N]_receive_Data
    SerialUSB.print((char)buffer[i]);
  SerialUSB.println("");
}

void loop(){
  toggleLED();
  delay(100);

}

