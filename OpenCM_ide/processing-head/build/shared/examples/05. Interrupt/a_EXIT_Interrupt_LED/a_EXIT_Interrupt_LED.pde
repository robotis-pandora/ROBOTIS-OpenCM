/*
 EXIT_Interrupt_LED
 
 Cicuit : 
   Toggle Button must be conneted Pin 5 with pull-down 3.3V
   ID 1 dynamixel is connected to TTL or RS485, anything is good.

 This example demonstrate how to use EXIT Interrupt with Dynamixel.
 If button is pushed, dynamixel connected moves to position 0. 
 If pushed again, it moves to position 500 repeatedly with LED blinking

 Created to CM9 series by ROBOTIS,.LTD.
*/

#define Cm9_butten 25
#define LED_ON 0
#define LED_OFF 1

void setup(){
  pinMode(BOARD_LED_PIN, OUTPUT);
  pinMode(Cm9_butten, INPUT_PULLDOWN);
  /*It can be choose as CHANGE, RISING or FALLING*/
  attachInterrupt(Cm9_butten,changeDirection, RISING);
}

void changeDirection(void){
    digitalWrite(BOARD_LED_PIN, LED_ON);
    delay(100);
}

void loop(){
  digitalWrite(BOARD_LED_PIN, LED_OFF);
}
