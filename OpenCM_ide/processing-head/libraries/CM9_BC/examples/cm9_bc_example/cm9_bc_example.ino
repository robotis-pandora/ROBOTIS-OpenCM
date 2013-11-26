#include <CM9_BC.h>
Dynamixel Dxl(3);
Dynamixel *pDxl = &Dxl;
BioloidController bioloid;

unsigned int cm9_bc_ex_id[] __FLASH__ = {1,19};

unsigned int backnforth_1[] __FLASH__ = {1, 512};
unsigned int backnforth_2[] __FLASH__ = {1, 768};
unsigned int backnforth_3[] __FLASH__ = {1, 256};

transition_t backnforth[] __FLASH__ = {{cm9_bc_ex_id,4},{backnforth_1,1000},{backnforth_2,1000},{backnforth_1,1000},{backnforth_3,1000}};



//##############################################################################
//##############################################################################
// Main function of User Program
//##############################################################################
//##############################################################################
void setup()
{
	Dxl.begin(1);
	SerialUSB.begin();

	// Waits 10 seconds for you to open the console (open too quickly after
	//   downloading new code, and you will get errors
    delay(10000);
	SerialUSB.print("Send any value to continue...\n");
	while(!SerialUSB.available())
	{
		delay(1000);
		digitalWrite(BOARD_LED_PIN, LOW);
		SerialUSB.print("Send any value to continue...\n");
		delay(1000);
		digitalWrite(BOARD_LED_PIN, HIGH);
	}
	SerialUSB.print("Now starting program\n");

	bioloid.setup(1);
	bioloid.setId(0, cm9_bc_ex_id[0+1]);
	SerialUSB.print("Done loading sequence.\n");
}

void loop()
{
	SerialUSB.print("Starting sequence...");
	bioloid.playSeq(backnforth);
//	SerialUSB.print("Starting loop.\n");
	while (bioloid.playing)
	{
		bioloid.play();
	}
	SerialUSB.print("done.\n");
}
