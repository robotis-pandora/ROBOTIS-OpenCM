/*
  LED_Fading
 
 This example shows how to fade an LED using the analogWrite() function.
 It operates only on OpenCM board, not CM-900(ES) because built-in led in CM900
 is not pwm-enabled pin, so you need some external circuit using other
 pwm-enabled pin.
 
 Created 1 Nov 2008
 By David A. Mellis
 Modified 17 June 2009
 By Tom Igoe
 
 Ported to CM9 15 Nov 2012
 by ROBOTIS,.LTD.
 
 Modified by LeafLabs for Maple
 Modified by ROBITIS for CM900 again
 
 http://arduino.cc/en/Tutorial/Fading
 
 For differences between Maple's analogWrite() and Arduino's analogWrite():
 http://leaflabs.com/docs/lang/api/analogwrite.html#arduino-compatibility
 */

// **** This example runs only on the OpenCM board!!****//
void setup() {  
  pinMode(BOARD_LED_PIN, PWM);  // setup the pin as PWM
}

void loop()  {
  // Fade in from min to max in increments of 1280 points:
  for (int fadeValue = 0; fadeValue <= 65535; fadeValue += 1280) {
    // Sets the value (range from 0 to 65535):
    analogWrite(BOARD_LED_PIN, fadeValue);
    // Wait for 30 milliseconds to see the dimming effect:
    delay(30);
  }

  // Fade out from max to min in increments of 1280 points:
  for (int fadeValue = 65535 ; fadeValue >= 0; fadeValue -= 1280) {
    // Sets the value (range from 0 to 1280):
    analogWrite(BOARD_LED_PIN, fadeValue);
    // Wait for 30 milliseconds to see the dimming effect:
    delay(30);
  }
}

