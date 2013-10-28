/*
 OLLO Gyro(Sensor) XY Read example
 
 Read  x,y ADC value from OLLO Gyro Sensor module on CM-904(not CM-900).
 connect Gyro Sensor Module(GS-12) to port 1.
 
 You can buy IR Sensor DYNAMIXEL in ROBOTIS-SHOP
 http://www.robotis-shop-en.com/shop/step1.php?number=833&b_code=B20070914051413&c_code=C20100528062452
 You can also find all information 
 http://support.robotis.com/
 
 Created to CM9 by ROBOTIS,.LTD. 30 MAY 2013
 */

#include <OLLO.h>
OLLO myOLLO;

void setup(){
  myOLLO.begin(1);//Gyro X Axis must be connected at port 2.
  myOLLO.begin(2);//Gyro Y Axis must be connected at port 2.
}
void loop(){
  SerialUSB.print("X-Axis ADC = ");
  SerialUSB.print(myOLLO.read(1)); //read X-Axis ADC value from OLLO port 1
  SerialUSB.print("  Y-Axis ADC = ");
  SerialUSB.println(myOLLO.read(2)); //read X-Axis ADC value from OLLO port 2
  delay(60);
}


