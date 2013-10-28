/*
Ultrasonic Library
*/
#include "wirish.h"
#include "Ultrasonic.h"
Ultrasonic::Ultrasonic(byte trigPin, byte echoPin)
{
  attachInterrupt(echoPin,timer_us,CHANGE);
  _trigPin = trigPin;
  _echoPin = echoPin;
}

short Ultrasonic::readDistance()
//Distance in cm between 0 and 150. 
{
  pinMode(_trigPin, OUTPUT);
  digitalWrite(_trigPin, LOW);  // Added this line
  delayMicroseconds(5); // Added this line
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(5); // Added this line
  pinMode(_echoPin, INPUT);
  return distance;
}

void Ultrasonic::timer_us()
{
  echoTime = micros()-startTime;
  startTime = micros();
  distance = timeDistanceConvert(echoTime, US_ROUNDTRIP_CM);
}




