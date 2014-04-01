#include <LiquidCrystal.h>
LiquidCrystal lcd(16, 17, 18, 19, 20, 21);
void setup() {
  // put your setup code here, to run once:
 lcd.begin(16, 2);
 lcd.setCursor(0, 0);
 lcd.print("OpenCM9.04");
}

void loop() {
  // put your main code here, to run repeatedly: 

}

