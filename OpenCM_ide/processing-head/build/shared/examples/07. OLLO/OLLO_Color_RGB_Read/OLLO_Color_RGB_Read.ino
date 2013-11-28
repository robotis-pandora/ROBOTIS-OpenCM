/*
 OLLO Color Sensor example
 
 Read RGB Color ADC value from OLLO Color Sensor module on OpenCM9.04.
 connect OLLO Color Module to port 1.
 
 Created to CM9 by ROBOTIS,.LTD. 30 MAY 2013
 */

#include <OLLO.h>
OLLO myOLLO;

void setup(){
  myOLLO.begin(1, COLOR_SENSOR);//Gyro X Axis must be connected at port 2.
 // myOLLO.begin(2);//Gyro Y Axis must be connected at port 2.
}
void loop(){
  SerialUSB.print("RED ADC = ");
  SerialUSB.print(myOLLO.read(1, COLOR_SENSOR, RED)); 
  SerialUSB.print("  GREEN ADC = ");
  SerialUSB.print(myOLLO.read(1, COLOR_SENSOR, GREEN));
  SerialUSB.print("  BLUE ADC = ");
  SerialUSB.println(myOLLO.read(1, COLOR_SENSOR, BLUE));
  
  delay(60);
}


