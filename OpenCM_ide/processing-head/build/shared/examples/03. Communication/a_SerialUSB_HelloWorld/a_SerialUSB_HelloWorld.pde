/*
 SerialUSB_HelloWorld
 USB Serial print "Hello World!!" to PC 
 You can see it any terminal program, putty, TeraTerm, Hyper Terminal, etc...
 
 created 14 Nov. 2012
 by ROBOTIS,.LTD.
 
*/
volatile int nCount=0;

void setup() {
  //Initialize USB Serial
  SerialUSB.begin();
}

void loop() {
  //print "Hello World!!" to PC though USB Virtual COM port
  SerialUSB.println("Hello World!!");
  SerialUSB.print("nCount : "); // display nCount variable and increase nCount.
  SerialUSB.println(nCount++);//SerialUSB.print("\r\n");  
  
  delay(1000);
}


