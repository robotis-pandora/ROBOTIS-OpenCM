/*
*   Whatever it receives from the Serial USB (usually commands and queries) 
 *  is sent to the Dynamixel bus, and what it receives from the Dynamixel bus
 *  is sent to the SerialUSB (usually answers):
 *  Created by aprendiendo
 *  http://softwaresouls.com/softwaresouls/2013/06/12/robotis-cm-900-as-a-tosser-for-dynamixel-commands/
 */

/*
*********************************************************************************
* Dynamixel instantiation on serial device
* To use dynamixel class, it must be needed for supporting OpenCM 485 SHD since v1.0.1
* Dynamixel bus can be operated on any serial device OpenCM board.
*********************************************************************************
*/
Dynamixel Dxl(1); //Dynamixel on Serial1(USART1)

int counter;
bool onlyOnceHappened;

void blinkOnce()
{
  digitalWrite(BOARD_LED_PIN, LOW);
  delay_us(100);
  digitalWrite(BOARD_LED_PIN, HIGH);
}

void setup()
{  
  pinMode(BOARD_LED_PIN, OUTPUT);

  onlyOnceHappened=false;
  counter=0;

  //USB Serial initialize
  SerialUSB.begin();
  //  SerialUSB.attachInterrupt(USBDataReceived);
  //DXL initialize
  Dxl.begin(1);  
}

byte aByte=0;
uint8 aUint8;

void loop() 
{    
  //  SerialUSB.println (counter++);

  if (onlyOnceHappened==false)
  {    
    blinkOnce();
    onlyOnceHappened=true;
    delay (3000); //Some time to the user to activate the monitor/console
    SerialUSB.println ("v1.1.1 Orders receiver started");  
  }    

  if (SerialUSB.available())
  {
    aUint8=SerialUSB.read();
    blinkOnce();
    Dxl.writeRaw(aUint8);
    //    delay(20);
  }   

  if (Dxl.available())
  {
    aByte=Dxl.readRaw();
    blinkOnce();
    SerialUSB.write(aByte);
    //    delay(20);
  } 
}


