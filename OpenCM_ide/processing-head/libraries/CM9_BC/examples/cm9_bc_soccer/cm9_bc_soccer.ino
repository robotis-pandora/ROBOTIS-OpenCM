#include <CM9_BC.h>
#include "HelloRobo_RPM.h"		// Only legs of Type-A Humanoid
Dynamixel Dxl(3);
Dynamixel *pDxl = &Dxl;
// Adjust for each bot
int Offset_Calibration[] __FLASH__ = {12,0,0,0,0,0,0,0,0,0,0,0,0};

#include <RC100.h>

BioloidController BioCon;
RC100 Boomer;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup()
{
	// Setup Dynamixel bus, USB, and RC-100 interfaces
	Dxl.begin(1);
	Dxl.setLibNumberTxRxAttempts(3);
	Dxl.writeWord(BROADCAST_ID, P_MAX_TORQUE_L, 300);
	
	SerialUSB.begin();
	Boomer.begin();

	while (1)
	{
		delay(1000);
		digitalWrite(BOARD_LED_PIN, LOW);
		SerialUSB.print("Send any value to continue...\n");
		delay(1000);
		digitalWrite(BOARD_LED_PIN, HIGH);
		if (SerialUSB.available())
		{
			SerialUSB.read();
			break;
		}
		if (Boomer.available())
		{
			break;
		}
	}
	SerialUSB.print("Now starting program\n");

	// Load the RoboPlusMotion_Array from our _RPM header file
	BioCon.RPM_Setup(HelloRobo_RoboPlusMotion_Array);
	//  This gives the motion engine the location of the RPM page file
	// 	Sets up the motion engine (allocates memory and initializes variables)
	//  Loads the servo IDs from the first pose in the first sequence in the array
	SerialUSB.print("Done with motion engine setup.\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop()
{
	unsigned int heartbeat = millis();

/// Set bot to an initial/ready position
	SerialUSB.print("loop() started. Going to Ready Position...");
//	BioCon.setTimeModifier(300);
	BioCon.loadOffsets(Offset_Calibration);
	BioCon.MotionPage(1);	// HelloRobo_Init_1
	while (BioCon.MotionStatus())
	{
		delay(1);
		BioCon.Play();

		if ((millis()-heartbeat)>200)
		{
			heartbeat = 0;
			SerialUSB.print(".");
		}

	}
	SerialUSB.print(" done.\nStarting main loop for user code.\n");
//	delay(5000);
//	BioCon.kill();
//	SerialUSB.print("BioCon killed.\n");
//	while(1);

/// Main loop where all user code occurs
// Do not modify anthing outside this while loop unless you are very, very
//  certain of what you are doing.
// Do not create any infinite loops or >30ms waits within this loop, or the
//  motion engine will fail to update.
	while(1)
	{
		unsigned int rang = 0;
		if (Boomer.available())
		{
			rang = Boomer.readData();
		}
		else if (SerialUSB.available())
		{
			rang = SerialUSB.read();
		}
		
		if (rang == RC100_BTN_U)
		{
			// Go forward
			BioCon.MotionPage(3);	// HelloRobo_F_R_S_3
		}
		else if (rang == RC100_BTN_D)
		{
			// Go Backward
			BioCon.MotionPage(13);	// HelloRobo_B_R_S_13
		}
		else if (rang == RC100_BTN_L)
		{
			// Strafe Left
			BioCon.MotionPage(24);	// HelloRobo_L_24
		}
		else if (rang == RC100_BTN_R)
		{
			// Strafe Right
			BioCon.MotionPage(23);	// HelloRobo_R_23
		}
		else if (rang == RC100_BTN_6)
		{
			// Spin Left
			BioCon.MotionPage(28);	// HelloRobo_LT_28
		}
		else if (rang == RC100_BTN_5)
		{
			// Spin Right
			BioCon.MotionPage(27);	// HelloRobo_RT_27
		}
		else if (rang == RC100_BTN_3)
		{
			// Go to Init
			BioCon.MotionPage(1);	// HelloRobo_Init_1
		}
		else if (rang == RC100_BTN_1)
		{
			// Defend Front
			BioCon.MotionPage(47);	// HelloRobo_D_Front_47
		}
		else if (rang == RC100_BTN_2)
		{
			// Defend Left
			BioCon.MotionPage(48);	// HelloRobo_D_Left_48
		}
		else if (rang == RC100_BTN_4)
		{
			// Defend Right
			BioCon.MotionPage(46);	// HelloRobo_D_Right_46
		}
		else if (rang > 0)
		{
			// Stop
			BioCon.MotionPage(0);	// INVALID/STOP
		}


		
		if ((millis()-heartbeat)>200)
		{
			heartbeat = millis();
			SerialUSB.print(".");

			if (BioCon.MotionStatus())
			{
				if (BioCon.MotionPage()>0)
				{
					SerialUSB.print("Playing page: ");
					SerialUSB.println(BioCon.MotionPage());
				}
			}
		}

/// DO NOT REMOVE ME!!!
		BioCon.Play();	// TODO: replace with timer interrupt-based update.
	}
}
