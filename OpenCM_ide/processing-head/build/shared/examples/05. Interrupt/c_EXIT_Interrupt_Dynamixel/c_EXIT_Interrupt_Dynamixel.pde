/*
 EXIT_Interrupt_Dynamixel
 
 External Interrupt example
 Cicuit : 
 Toggle Button must be conneted Pin 5 with pull-down 3.3V
 ID 1 dynamixel is connected to TTL or RS485, anything is good.
 
 This example demonstrate how to use EXIT Interrupt with Dynamixel.
 If button is pushed, dynamixel connected moves to position 0. 
 If pushed again, it moves to position 500 repeatedly with LED blinking
 
 Created to CM9 series by ROBOTIS,.LTD.
 */
//Dxl.writeWord(Dynamixel_Motor_Number, Address_Number, Address_Data);
#define ID_Num 1

/* Address_Number_Define */
#define Goal_Postion 30
#define Moving_Speed 32

#define Cm9_butten 25
#define LED_ON 0
#define LED_OFF 1

Dynamixel Dxl(1);//Instanciate dynamixel on Serial1(USART1)
volatile unsigned ExitFlag=1;

void setup(){

  SerialUSB.begin();
  pinMode(BOARD_LED_PIN, OUTPUT);
  pinMode(Cm9_butten, INPUT_PULLDOWN);
  /*It can be choose as CHANGE, RISING or FALLING*/
  attachInterrupt(Cm9_butten,changeDirection, RISING);
  Dxl.begin(1);  //1Mbps

}

void changeDirection(void){
  SerialUSB.print("Dxl_!\r\n");
  ExitFlag=1; 
}

void loop(){
  toggleLED();
  delay(100);

  if(ExitFlag==1)
  { 
    Dxl.writeWord(ID_Num, Goal_Postion, 0); //Turn dynamixel ID 1 to position 0
    delay(500);              // Wait for 1 second (1000 milliseconds)
    Dxl.writeWord(ID_Num, Goal_Postion, 300);//Turn dynamixel ID 1 to position 300
    delay(500);              // Wait for 1 second (1000 milliseconds)

    ExitFlag=0;
  }
}

