/*
* Wii Nunchuck example by Martin Mason http://www.profmason.com
* based on work by chad phillips http://www.windmeadow.com/node/42
* Be sure to pull up your two I2C ports high with ~2K resistors to the 3.3V line.  
* By default using the pins 14 and 15 for SDA and SCL.
*/

#include <Wire.h>
#undef int

uint8_t outbuf[6];		// array to store nunchuk output
int cnt = 0;                    // count through the data back from the nunchuk

void
setup ()
{
  SerialUSB.begin ();
  SerialUSB.print ("Finished setup\n");
  Wire.begin (14,15);		// join i2c bus with address 0x52
  delay(100);
  nunchuck_init (); // write the initilization handshake
  delay(2);
}

void nunchuck_init ()
{
  //New Initialization No Decrypt required!
  Wire.beginTransmission(0x52);       // device address
  Wire.write(0xF0);
  Wire.write(0x55);
  Wire.endTransmission();
  delay(1);
  Wire.beginTransmission(0x52);
  Wire.write(0xFB);

  Wire.write(0x01);
  Wire.write((uint8_t)0x00);

  Wire.endTransmission();
  
  
  
}

void write_zero ()
{
  Wire.beginTransmission (0x52);	// transmit to device 0x52
  Wire.write (0x00);		// writes one byte
  Wire.endTransmission ();	// stop transmitting
}

void loop ()
{
  //nunchuck_init ();
  
  Wire.requestFrom (0x52, 6);	// request data from nunchuck
  delayMicroseconds(200);
  while (Wire.available ())
    {
      outbuf[cnt] = nunchuk_decode_byte (Wire.read ());
      cnt++;
    }

   //If we recieved the 6 bytes, then go print them
  if (cnt >= 6)
    {
      print ();
    }
//
  cnt = 0;
  write_zero (); // write the request for next bytes
  delayMicroseconds (300);
}

// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
void print ()
{
  int joy_x_axis = outbuf[0];
  int joy_y_axis = outbuf[1];
  int accel_x_axis = outbuf[2] * 2 * 2; 
  int accel_y_axis = outbuf[3] * 2 * 2;
  int accel_z_axis = outbuf[4] * 2 * 2;

  int z_button = 0;
  int c_button = 0;

 // byte outbuf[5] contains bits for z and c buttons
 // it also contains the least significant bits for the accelerometer data
 // so we have to check each bit of byte outbuf[5]
  if ((outbuf[5] >> 0) & 1)
    {
      z_button = 1;
    }
  if ((outbuf[5] >> 1) & 1)
    {
      c_button = 1;
    }

  if ((outbuf[5] >> 2) & 1)
    {
      accel_x_axis += 2;
    }
  if ((outbuf[5] >> 3) & 1)
    {
      accel_x_axis += 1;
    }

  if ((outbuf[5] >> 4) & 1)
    {
      accel_y_axis += 2;
    }
  if ((outbuf[5] >> 5) & 1)
    {
      accel_y_axis += 1;
    }

  if ((outbuf[5] >> 6) & 1)
    {
      accel_z_axis += 2;
    }
  if ((outbuf[5] >> 7) & 1)
    {
      accel_z_axis += 1;
    }

  SerialUSB.print (joy_x_axis, DEC);
  SerialUSB.print ("\t");

  SerialUSB.print (joy_y_axis, DEC);
  SerialUSB.print ("\t");

  SerialUSB.print (accel_x_axis, DEC);
  SerialUSB.print ("\t");

  SerialUSB.print (accel_y_axis, DEC);
  SerialUSB.print ("\t");

  SerialUSB.print (accel_z_axis, DEC);
  SerialUSB.print ("\t");

  SerialUSB.print (z_button, DEC);
  SerialUSB.print ("\t");

  SerialUSB.print (c_button, DEC);
  SerialUSB.print ("\t");

  SerialUSB.print ("\r\n");
}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
char nunchuk_decode_byte (char x)
{
  //x = (x ^ 0x17) + 0x17;
  return x;
}
