/*
 OLLO Touch sensor interrupt example
 
 When push OLLO Touch sensor module, toggles built-in LED using external interrupt on CM-904.
 connect OLLO Touch sensor module to port 2.
  
 Created to CM9 by ROBOTIS,.LTD. 30 MAY 2013
 */

#include <OLLO.h>
OLLO myOLLO;

void setup(){
  myOLLO.begin(2, TOUCH_SENSOR);//OLLO Touch sensor Module must be connected at port 2.
  
}
void loop(){
  SerialUSB.print("Touch Read = ");
  SerialUSB.println(myOLLO.read(2, TOUCH_SENSOR));
  delay(100); //very important delay to prevent panic.
}




