/*
 *******************************************************************************
 *  HaViMo2 Pan-Tilt Tracking Example (CM-900 version)
 *******************************************************************************
 *  -A very basic program to track an object using the HaViMo2 vision module
 *    mounted on a pan-tilt dynamixel turret and all controlled by a CM-900.
 *******************************************************************************
 *  -A far more in-depth Embedded-C based introduction to using the HaViMo2
 *    from its creator is at:
 *      <http://robosavvy.com/site/Builders/hamid_m/example.c>
 *    That CM-5 example source code was posted on the RoboSavvy forums in the
 *    "Bioloid Vision Module" thread (page 4, post 9).
 *      <http://robosavvy.com/forum/viewtopic.php?t=1341&postdays=0&postorder=asc&start=45>
 *******************************************************************************
 */

#include <HaViMo2.h>

//#define VERBOSE_IMAGE_PROCESSING
//#define VERBOSE_IMAGE_TRACKING
#define DXL_PAN_SERVO                   19
#define DXL_TILT_SERVO                  20


HaViMo2_Controller hvm2;

uint8_t Targetx, Targety;
uint8_t TrackFound;


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

	Dxl.writeWord(DXL_PAN_SERVO, AXM_CW_ANGLE_LIMIT_L, 0);
	Dxl.writeWord(DXL_PAN_SERVO, AXM_CCW_ANGLE_LIMIT_L, 1023);
	Dxl.writeWord(DXL_TILT_SERVO, AXM_CW_ANGLE_LIMIT_L, 128);
	Dxl.writeWord(DXL_TILT_SERVO, AXM_CCW_ANGLE_LIMIT_L, 556);
	SerialUSB.print("Send 'T' to start tracking and any other value to stop\n");
}

void loop()
{
// Main Code for Testing HaViMo2
	uint16_t HeadVPos, HeadHPos;
	char input=0;

//	Dxl.writeByte(DXL_PAN_SERVO, AXM_TORQUE_ENABLE, 1);
//	Dxl.writeByte(DXL_TILT_SERVO, AXM_TORQUE_ENABLE, 1);
	Dxl.writeWord(DXL_PAN_SERVO, AXM_GOAL_POSITION_L, 512);
	Dxl.writeWord(DXL_TILT_SERVO, AXM_GOAL_POSITION_L, 512);

	delay(2000);


	while(1) 
	{
		// For debug/testing without a serial connection and keyboard
		delay(100);
		while (!SerialUSB.available());
		input = SerialUSB.read();
		delay(2000);

	//Track Target
		if (input=='T')
		{
			HeadHPos=512;
			HeadVPos=512;
			SerialUSB.print("\nStart tracking\n");

		// Track Indefinitely
			while (!SerialUSB.available())
			{
				// Colors are:
				//   0/Unknown/Black, 1/Ball/Teal, 2/Field/Red, 3/MyGoal/Green,
				//   4/OppGoal/Purple, 5/Robot/White, 6/Cyan, 7/Magenta
				if (ProcessImage(Teal))
				{
					if (TrackFound)
					{
						HeadHPos-=((int)Targetx-80)/2;
						HeadVPos-=((int)Targety-60)/2;
						Dxl.writeWord(DXL_PAN_SERVO, AXM_GOAL_POSITION_L, HeadHPos);
						Dxl.writeWord(DXL_TILT_SERVO, AXM_GOAL_POSITION_L, HeadVPos);
#ifdef VERBOSE_IMAGE_TRACKING
						SerialUSB.print("\n Target X,Y: ");
						SerialUSB.print(Targetx);
						SerialUSB.print(",");
						SerialUSB.print(Targety);
#endif
					}
#ifdef VERBOSE_IMAGE_PROCESSING
					SerialUSB.print("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif
				}
			}
			SerialUSB.print("\nDone tracking\n");
		}
	}
}



//##############################################################################
//##############################################################################
// Function to retrieve (x,y) coordinates of tracked color
//##############################################################################
//##############################################################################
int ProcessImage(uint8_t TrackingColor)
{
	TrackFound=0;

	if (!hvm2.ready())
	{
		return 0;
	}

// Recover current Region Buffer
	uint8_t valid_regions = hvm2.recover();
#ifdef VERBOSE_IMAGE_PROCESSING
	SerialUSB.print("\nNumber of Color Regions found: ");
	SerialUSB.print(valid_regions);
#endif
// Start capture of next image
	hvm2.capture();


	uint8_t iter;
	int matches = 0;

	// Examine the Region Buffer
	for (iter=0; iter<valid_regions; iter++)
	{
		if (hvm2.color(iter)==TrackingColor)
		{
			matches++;

#ifdef VERBOSE_IMAGE_PROCESSING
			if (matches == 1)
			{
				SerialUSB.print("\nTracked Color Matched in Region: ");
			}
			SerialUSB.print(iter+1);
			SerialUSB.print(" ");
#endif
			// bigger than the last region found
			if(hvm2.size(iter)>TrackFound)
			{
				Targetx=hvm2.avgX(iter);
				Targety=hvm2.avgY(iter);
				TrackFound=hvm2.size(iter);
			}
		}
	}
#ifdef VERBOSE_IMAGE_PROCESSING
	SerialUSB.print("\n");
#endif
	return matches;
}
