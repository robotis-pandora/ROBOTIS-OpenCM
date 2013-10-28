/**********************************************
Controlling a servo position using a potentiometer (variable resistor) 
by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 
Ported to CM900 by Martin Mason http://www.profmason.com
Recommended Servos are 20,21,22,23
**********************************************/

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
 
int potpin = 1;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin 
 
void setup() 
{ 
  myservo.attach(20);  // attaches the servo on pin 20 to the servo object 
} 
 
void loop() 
{ 
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 4096, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
} 
