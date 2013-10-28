/*
 OLLO DMS Sensor Read example
 
 Read ADC value from OLLO DMS Sensor module on CM-904(not CM-900).
 connect DMS Sensor Module(DMS-80) to port 1.
 
 You can buy IR Sensor DYNAMIXEL in ROBOTIS-SHOP
 http://www.robotis-shop-en.com/shop/step1.php?number=834&b_code=B20070914051413&c_code=C20100528062452
 You can also find all information 
 http://support.robotis.com/
 
 Created to CM9 by ROBOTIS,.LTD. 30 MAY 2013
 */

#include <OLLO.h>
OLLO myOLLO;

void setup(){
  myOLLO.begin(2);//DMS Module must be connected at port 2.
}
void loop(){
  SerialUSB.print("DMS Sensor ADC Value = ");
  SerialUSB.println(myOLLO.read(2)); //read ADC value from OLLO port 2
  delay(60);
}



