/*
 OLLO Color Sensor example
 
 Read RGB Color ADC value from OLLO Color Sensor module on OpenCM9.04.
 connect OLLO Color Module to port 1.
 
 Created to CM9 by ROBOTIS,.LTD. 30 MAY 2013
 */

#include <OLLO.h>
OLLO myOLLO;


void setup(){
//myOLLO.begin( Port Number, sensor device index );
  myOLLO.begin(1, COLOR_SENSOR);//Color sensor module must be connected to port 1.

}
void loop(){
  SerialUSB.print("RED ADC = ");
  SerialUSB.print(myOLLO.read(1, COLOR_SENSOR, RED)); //myOLLO.read(port number, dev_index, color index)
  SerialUSB.print("  GREEN ADC = ");
  SerialUSB.print(myOLLO.read(1, COLOR_SENSOR, GREEN));
  SerialUSB.print("  BLUE ADC = ");
  SerialUSB.println(myOLLO.read(1, COLOR_SENSOR, BLUE));
  
  delay(60);
}


