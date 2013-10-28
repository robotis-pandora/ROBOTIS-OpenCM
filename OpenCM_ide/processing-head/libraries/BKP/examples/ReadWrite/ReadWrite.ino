/*
  BKP Register Example
 
 Demonstrates how to use Backup Register(BKP) in CM9 board.
 The backup register are ten 16-bit registers for storing 20 bytes
 of user application data. 
 BKP can keep user data after reset the board(not power-off).
 But if you turn off the power to CM9 board, the data will be lost.
 created 19 Dec 2012 by ROBOTIS,.LTD.
 */

#include "BKP.h"
BKP BKP_MEMORY;
int i;
void setup(){
  /* Initialize clock and registers for BKP*/
  BKP_MEMORY.begin();
  /* If you want to write data to BKP registers, you need to execute enable method */
  BKP_MEMORY.enable();
  for(i=1; i<11;i++){
    BKP_MEMORY.write(i,i*10);//CM-900 and CM-904 has only 1~10 BKP registers(16-bit)
  }
  /* After writing data, must be disabled writing data to BKP Register*/
  BKP_MEMORY.disable();
}
void loop(){
  delay(1000);
  for(i=1; i<11;i++){
    /* You can check the data written in setup() using SerialUSB*/
    SerialUSB.print("BKP Memory example = "); 
    /* Just to read data from BKP, pass BKP register number(1~10) to read() method */
    SerialUSB.println(BKP_MEMORY.read(i));
  }
}


