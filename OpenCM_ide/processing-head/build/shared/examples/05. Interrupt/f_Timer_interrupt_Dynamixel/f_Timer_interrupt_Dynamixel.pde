/*

 */
//Dxl.writeWord(Dynamixel_Motor_Number, Address_Number, Address_Data);
#define ID_Num 1

/* Address_Number_Define */
#define Goal_Postion 30

#define LED_RATE 1000000    // in microseconds; should give 0.5Hz toggles
volatile unsigned int TimSec=0, DxlFlag=0, Pos=0;

Dynamixel Dxl(1);//Instanciate dynamixel on Serial1(USART1)
HardwareTimer Timer(1);// Instanciate HardwareTimer class on timer device 1

void setup() {
  Dxl.begin(1); // init bus baudrate as 1Mbps
  // Set up the LED to blink
  pinMode(BOARD_LED_PIN, OUTPUT);
  //SerialUSB Serial initialize
  SerialUSB.begin();

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
  Dxl.writeWord(ID_Num, Goal_Postion, Pos);
}

void handler_led(void) {
  toggleLED(); 
  Pos=Pos+100;
  if(Pos>1023)Pos=0;

}

