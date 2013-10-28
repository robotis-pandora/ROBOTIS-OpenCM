#include <Wire.h>
#include <BMP085.h>

/*************************************************** 
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Based on work by Limor Fried/Ladyada for Adafruit Industries.  https://www.adafruit.com/products/391
  Ported to the CM900 by Martin S. Mason  http://profmason.com
 ****************************************************/

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on CM900 thats Pin 21
// Connect SDA to i2c data - on CM900 thats Pin 20
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

BMP085 bmp;
  
void setup() {
  SerialUSB.begin();
  bmp.begin(14,15);  
}
  
void loop() {
    SerialUSB.print("Temperature = ");
    SerialUSB.print(bmp.readTemperature());
    SerialUSB.println(" *C");
    
    SerialUSB.print("Pressure = ");
    SerialUSB.print(bmp.readPressure());
    SerialUSB.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    SerialUSB.print("Altitude = ");
    SerialUSB.print(bmp.readAltitude());
    SerialUSB.println(" meters");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    SerialUSB.print("Real altitude = ");
    SerialUSB.print(bmp.readAltitude(101500));
    SerialUSB.println(" meters");
    
    SerialUSB.println();
    delay(500);
}