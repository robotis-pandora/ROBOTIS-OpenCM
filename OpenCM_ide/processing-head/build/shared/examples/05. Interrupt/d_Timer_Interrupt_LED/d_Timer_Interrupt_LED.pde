/*
 Timer Interrupt Example (CM-9 version)
 This program is based on Maple tutorial ( www.leaflabs.com )
 You can learn how to implement timer interrupt in cm-9 IDE  
 modified by ROBOTIS,.LTD. 2012-12-29  
 */

#define LED_RATE 100000    // in microseconds; should give 0.5Hz toggles

HardwareTimer Timer(1);

void setup() {
  // Set up the LED to blink
  pinMode(BOARD_LED_PIN, OUTPUT);

  // Pause the timer while we're configuring it
  Timer.pause();

  // Set up period
  Timer.setPeriod(LED_RATE); // in microseconds

  // Set up an interrupt on channel 1
  Timer.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  Timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
  Timer.attachInterrupt(TIMER_CH1, handler_led);

  // Refresh the timer's count, prescale, and overflow
  Timer.refresh();

  // Start the timer counting
  Timer.resume();
}

void loop() {
  // Nothing! It's all in the handler_led() interrupt:
}

void handler_led(void) {
  toggleLED();
}

