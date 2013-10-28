/*
 OLLO IR Sensor Read example
 
 Read ADC value from OLLO IR Sensor module on CM-904(not CM-900).
 connect IR Sensor Module(OIS-10) to port 1.
 
 You can buy IR Sensor DYNAMIXEL in ROBOTIS-SHOP
 http://www.robotis-shop-en.com/shop/step1.php?number=750&b_code=B20070914051413&c_code=C20100528062452
 You can also find all information 
 http://support.robotis.com/
 
 Created to CM9 by ROBOTIS,.LTD. 30 MAY 2013
 */

#include <OLLO.h>
OLLO myOLLO;

void setup(){
  myOLLO.begin(1, IR_SENSOR);//IR Module must be connected at port 1.
}
void loop(){
  SerialUSB.print("IR Sensor ADC = ");
  SerialUSB.println(myOLLO.read(1, IR_SENSOR)); //read ADC value from OLLO port 1
  delay(60);
}



