/*
  DigitalReadSerial
 Reads a digital input on pin 23, prints the result to the serial monitor 
 You can use BOARD_BUTTON_PIN instead of using pin 23 in OpenCM9.04
 
 Test Guide
 1. Push button is connect to BOARD_BUTTON_PIN
 2. After Download OpenCM9 source
 3. Open Moniter (Control+Shift+M)
 
 created 16 Nov 2012
 by ROBOTIS,.LTD.
 */

// **** This example runs only on the OpenCM board, not CM-900!!****//
// BOARD_BUTTON_PIN has a pushbutton attached to it. Give it a name:

void setup(){
  //SerialUSB Serial initialize
  SerialUSB.begin(); 
  pinMode(BOARD_BUTTON_PIN, INPUT_PULLDOWN);
}
void loop(){
  int buttonState = digitalRead(BOARD_BUTTON_PIN);
  SerialUSB.print("buttonState = ");
  SerialUSB.println(buttonState);//SerialUSB.print("\r\n");
  delay(100);
}


