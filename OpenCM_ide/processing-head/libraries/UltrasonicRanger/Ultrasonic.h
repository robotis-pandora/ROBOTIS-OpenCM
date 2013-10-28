//Ultrasonic Library  www.profmason.com
#define US_ROUNDTRIP_CM 57
#define timeDistanceConvert(echoTime, conversionFactor) (max((echoTime + conversionFactor / 2) / conversionFactor, (echoTime ? 1 : 0)))

#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "wirish.h"

volatile static long echoTime;
volatile static long startTime;
volatile static byte distance;



class Ultrasonic
{
    public:
        Ultrasonic(byte trigPin, byte echoPin);
        short readDistance();
    private:
        static void timer_us();
        byte _trigPin;
        byte _echoPin;
       
};

#endif