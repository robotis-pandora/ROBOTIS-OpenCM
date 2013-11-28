/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to
 65535 and uses the result to set the pulse width modulation (PWM) of
 an output pin.  Also prints the results to the serial monitor.

 The circuit:
 * Potentiometer connected to analog pin 1.
 Center pin of the potentiometer goes to the analog pin.
 Side pins of the potentiometer go to +3.3V and ground.
 * LED connected from digital pin 14 to 3.3V( defined as BOARD_LED_PIN )
 If you have CM-900, you have to connect LED to any PWM enable pin.
 
 created 29 Dec. 2008 by Tom Igoe (LeafLabs)
 
 ported from Maple to OpenCM by ROBOTIS,.LTD.
 */

// These constants won't change.  They're used to give names
// to the pins used:

const int analogInPin = 2; // Analog input pin that the potentiometer

// These variables will change:
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM

// **** This example runs only on the OpenCM board, not CM-900!!****//

void setup() {
  // Configure the ADC pin
  pinMode(analogInPin, INPUT_ANALOG);
  // Configure LED pin
  pinMode(BOARD_LED_PIN, PWM);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 4095, 0, 65535);
  // change the analog out value:
  analogWrite(BOARD_LED_PIN, outputValue);

  // print the results to the serial monitor:
  SerialUSB.print("sensor = " );
  SerialUSB.print(sensorValue);
  SerialUSB.print("\t output = ");
  SerialUSB.println(outputValue);
  delay(100);
}


