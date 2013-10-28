#include <CM9_BC.h>

//#include "bio_prm_soccertypea_en.h"			// Full Type-A Humanoid
#include "bio_prm_legs_soccertypea_en.h"		// Only legs of Type-A Humanoid

BioloidController bioloid;

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

	// Can load servo IDs from any transition_t sequence produced by mtn2cm9.py
	unsigned int *servo_ids = cm9_bc_Init[0].pose;
	unsigned int num_servos = servo_ids[0];
	bioloid.setup(num_servos);
	int iter;
	for (iter=0; iter<num_servos; iter++)
	{
		bioloid.setId(iter, servo_ids[iter+1]);
		SerialUSB.print("Servo ");
		SerialUSB.print(iter);
		SerialUSB.print(" is ID ");
		SerialUSB.println(bioloid.getId(iter));//servo_ids[iter+1]);
	}
	SerialUSB.print("Done setting servo IDs.\n");
//	while (1);
}

void loop()
{
/*
	SerialUSB.print("Starting sequence...");
	bioloid.playSeq(cm9_bc_Init);
	while (bioloid.playing)
	{
		bioloid.play();
	}
	SerialUSB.print("done.\n");
*/
	SerialUSB.print("Going to initial pose...");
	bioloid.loadPose(cm9_bc_Init_1);
	bioloid.interpolateSetup(100);
	while(bioloid.interpolating > 0)
	{
		bioloid.interpolateStep();
		delay(1000);
	}
	SerialUSB.print("done.\n");

	while(1);
}
