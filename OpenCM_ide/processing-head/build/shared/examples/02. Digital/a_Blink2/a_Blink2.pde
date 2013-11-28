/*
 Blink2
 
 Turns on the built-in(Status) LED on for 0.1 second, then off for 0.1 second,
 repeatedly using toggleLED() which is function only for built-in LED.
 
 Ported to OpenCM Series from the Arduino example 27 May 2011
 By ROBOTIS,.LTD.
 */

void setup() {
  // Set up the built-in LED pin as an output:
  pinMode(BOARD_LED_PIN, OUTPUT);
}

void loop() {
  toggleLED();  //toggle digital value based on current value.
  delay(100);   //Wait for 0.1 second
}

