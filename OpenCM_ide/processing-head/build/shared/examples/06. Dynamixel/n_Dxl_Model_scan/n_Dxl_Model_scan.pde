/*
 *******************************************************************************
 *  Dynamixel Bus Scanner (CM-900 version)
 *******************************************************************************
 *  Searches through all valid IDs to find all dynamixel devices currently on
 *    the bus, and uses the Model Number to identify the device by name
 *******************************************************************************
 */
// My gigantic dynamixel header file
#include <HaViMo2.h>

/*
*********************************************************************************
* Dynamixel instantiation on serial device
* To use dynamixel class, it must be needed for supporting OpenCM 485 SHD since v1.0.1
* Dynamixel bus can be operated on any serial device OpenCM board.
*********************************************************************************
*/
Dynamixel Dxl(1); //Dynamixel on Serial1(USART1)
Dynamixel *pDxl = &Dxl;

#define DXL_BAUDRATE_NUMBER 1  // number = 1 is 1000000 bps, 34 is 57600 bps

void setup()
{
	SerialUSB.begin();
	Dxl.begin(DXL_BAUDRATE_NUMBER);
	Dxl.setLibNumberTxRxAttempts(5);

	pinMode(BOARD_LED_PIN, OUTPUT);

	// Waits 5 seconds for you to open the console (open too quickly after
	//   downloading new code, and you will get errors
	delay(5000);
	SerialUSB.print("Send any value to continue...\n");
	while(!SerialUSB.available())
	{
		delay(1000);
		digitalWrite(BOARD_LED_PIN, LOW);
		SerialUSB.print("Send any value to continue...\n");
		delay(1000);
		digitalWrite(BOARD_LED_PIN, HIGH);
	}
}

void loop()
{
	int model;
	for (int i=1; i<BROADCAST_ID; i++)
	{
		// Print device ID being checked
		SerialUSB.print(i);
		delay(10);
		// Read model number
		model = Dxl.readWord(i, AXM_MODEL_NUMBER_L);
		// If got a response, then device exists
		
		if (Dxl.getResult()==(1<<COMM_RXSUCCESS))
		{
			if (model == MODEL_AX12)
				SerialUSB.println(": AX-12");
			else if (model == MODEL_AX12W)
				SerialUSB.println(": AX-12W");
			else if (model == MODEL_AX18)
				SerialUSB.println(": AX-18");
			else if (model == MODEL_AXS1)
				SerialUSB.println(": AX-S1");
			else if (model == MODEL_AXS20)
				SerialUSB.println(": AX-S20");
			else if (model == MODEL_JHFPS)
				SerialUSB.println(": JH-FPS");
			else if (model == MODEL_HaViMo2)
				SerialUSB.println(": HaViMo2");
			else if (model == MODEL_MX28)
				SerialUSB.println(": MX-28");
			else if (model == MODEL_IRSA)
				SerialUSB.println(": IRSA");
			else
			{
				// Not recognized/supported by my dynamixel device header file
				SerialUSB.print(": Unknown : ");
				SerialUSB.println(model);
			}
		}
		else
			SerialUSB.println(": Not Found");
	}
	// Indicate search is finished by toggling the LED endlessly
	while(1)
	{
		digitalWrite(BOARD_LED_PIN, LOW);
		delay(100);
		digitalWrite(BOARD_LED_PIN, HIGH);
		delay(100);
	}
}

