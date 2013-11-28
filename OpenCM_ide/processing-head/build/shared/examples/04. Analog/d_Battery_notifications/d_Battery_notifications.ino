
/******************
Battery notifications

This example measures charge of 11.1v 1Ah LiPo battery from Bioloid Premium Kit.

Based in Arduino Cookbook (O’Reilly) Examples
By Luis Samahí García González
*******************/

/*You need to do your own voltage divider Vout = (R2/(R2+R1))*Vin */

const int An_CMpin = 0; //Pin A0 in CM-900

void setup(){
pinMode(BOARD_LED_PIN,OUTPUT);
pinMode(An_CMpin, INPUT_ANALOG); // Set Analog input for pin A0
}

void loop(){
  float val = analogRead(An_CMpin)/10; // we read values from our battery
  
  //SerialUSB.println(val);
  /* we do a little conversion, 910 is the result of (4095/(R2(R2+R1)))/10 */
  float BatVolts = ((val)/(910)); 
  float VoltsMul = BatVolts * 11.1;
 
   if(VoltsMul > 4 && VoltsMul < 5){
/*Notification to know your battery is ok.*/
     digitalWrite(BOARD_LED_PIN,0);
     delay(100);
     digitalWrite(BOARD_LED_PIN,1);
     delay(100);
   }

delay(500); // An small delay after each analog reading.
}
