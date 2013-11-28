/*
  AnalogReadSerial
 Reads an analog input on pin 0, prints the result to the serial monitor.
 
 This example code is in the public domain.
 */



void setup(){
  //Serial2 Serial initialize
  SerialUSB.begin(); 
}

void loop(){
  int AdcData = analogRead(0);
  SerialUSB.print("AdcData = ");
  SerialUSB.println(AdcData);//SerialUSB.print("\r\n");
  delay(100);
}

