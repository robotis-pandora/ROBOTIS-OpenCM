// Use the Ultrasonic distance sensors such as the HC-SR04 
// Typical ranges are from 4cm to 150 cm. 
// Connect VCC of the sensor to 5.0V
// Connect GND to Ground
// Note Library uses an Interrupt so is non-blocking but don't call sensors more often then every 10ms 
// as this will cause interference between different sensors 
// Connect Trig to Pin 8 and Echo to Pin 7, but can use any digital pins. 
// Written for the CM900 by Martin S. Mason  http://profmason.com

#include <Ultrasonic.h>

Ultrasonic leftSensor(8,7);  //Specify Trig and Echo pins (Trig,Echo)  

void setup() {
  SerialUSB.begin();                             // start the serial port
}

void loop() {
  SerialUSB.print("LDistance ");
  SerialUSB.println(leftSensor.readDistance());  //Read and Display the Distance
  delay(20);
}




