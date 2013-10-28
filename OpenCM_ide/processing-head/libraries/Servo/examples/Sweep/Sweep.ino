/**************************************
Sweep
by BARRAGAN <http://barraganstudio.com> 
ported to CM900 by Martin Mason http://www.profmason.com
This example code is in the public domain.
minimum pulse length is 550ns maximum is 2400 ns

Recommend to use Servos on Pins 20,21,22 and 23
Valid Servos based on test are 0,1,3,6,7,8,10,14,15,20,21,22,23
Servos 2 and 9 should work but YMMV
Timer 1 8,9,10 
Timer 2 0,1,2,3
Timer 3 6,7,14,15
Timer 4 20,21,22,23
****************************************/

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  myservo.attach(20);  // attaches the servo on pin 20 to the servo object 
} 
 
 
void loop() 
{ 
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);// tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);// tell servo to go to position in variable 'pos'  
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
} 
