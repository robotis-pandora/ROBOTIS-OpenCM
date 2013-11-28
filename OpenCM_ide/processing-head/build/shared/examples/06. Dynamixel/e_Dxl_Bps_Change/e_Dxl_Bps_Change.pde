/*
 Dynamixel Baud Change Example
 
 This example shows how to set baud rate and ID using broadcast ID
 All dynamixel in bus set to as 1Mbps , ID =1
 If you want your all dxl to reset ID and baud rate concurrentely.
 utilize this example.
 
 ID=0xfe is broadcast ID, refer to ROBOTIS E-manual
 Address = 3 : ID Change
 Address = 4 : Baud Rate
 Address = 4, Data = 1 : 1000000bps (0.000%)
 Address = 4, Data = 3 : 500000bps (0.000%)
 Address = 4, Data = 4 : 400000bps (0.000%)
 Address = 4, Data = 7 : 250000bps (0.000%)
 Address = 4, Data = 9 : 200000bps (0.000%)
 Address = 4, Data = 16 : 115200bps (-2.124%)
 Address = 4, Data = 34 : 57600bps (0.794%)
 Address = 4, Data = 103 : 19200bps (0.160%)
 Address = 4, Data = 207 : 9600bps (0.160%)
 
 The sequence is described as the below
 1. init dxl bus as 57600bps
 2. All dxls are set to be ID = 1
 3. New baud rate is set to be 1 Mbps
 4. After above changement, it is successfull if dxl moves well
 */
/*
*********************************************************************************
* Dynamixel instantiation on serial device
* To use dynamixel class, it must be needed for supporting OpenCM 485 SHD since v1.0.1
* Dynamixel bus can be operated on any serial device OpenCM board.
*********************************************************************************
*/
Dynamixel Dxl(1); //Dynamixel on Serial1(USART1)

void setup() {
  // Initialize the dynamixel bus:
  Dxl.begin(34);  //57600bps
  Dxl.writeByte(0xfe, 3, 1); //Turn dynamixel ID 1 to position 0
  Dxl.writeByte(0xfe, 4, 1);  //Baud rate set to 1 Mbps
  // Re-initialize dynamixel bus as 1Mbps
  Dxl.begin(1);  // initialize again as changed baud rate 1Mbps  
}

void loop() {
  delay(500);              // Wait for 0.5 second (500 milliseconds)
  Dxl.writeWord(1, 30, 90); //Turn dynamixel ID 1 to position 0
  delay(500);              // Wait for 0.5 second 
  Dxl.writeWord(1, 30, 100);//Turn dynamixel ID 1 to position 300
}




