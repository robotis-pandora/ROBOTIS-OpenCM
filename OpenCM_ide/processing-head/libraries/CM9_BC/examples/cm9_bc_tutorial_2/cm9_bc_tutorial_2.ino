#include <CM9_BC.h>

// This example is to demonstrate how to use the CM9_BC pose and manual
//  interpolation between poses.  It can be run on a carbot, but the servo
//  position values may cause issues on other robots.
Dynamixel Dxl(3);
Dynamixel *pDxl = &Dxl;
bc_pose_t ServoIDs[] __FLASH__ = {4,1,2,3,4};

bc_pose_t Middle_Mid[] __FLASH__ = {4,512,512,512,512};
bc_pose_t Middle_Max[] __FLASH__ = {4,768,768,768,768};
bc_pose_t Middle_Min[] __FLASH__ = {4,256,256,256,256};

bc_seq_t Sweeper[] __FLASH__ = {{ServoIDs,4}, {Middle_Max,500}, {Middle_Mid,500}, {Middle_Min,500}, {Middle_Mid,500}};

BioloidController BioCon;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup()
{
	// Setup Dynamixel bus and USB interfaces
	Dxl.begin(1);
	SerialUSB.begin();

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
	}
	SerialUSB.print("Now starting program\n");

	SerialUSB.print("Starting BioloidController object setup.\n  Allocating memory and initializing internal variables.\n");
	// The number of servos in a pose is always the first entry in a bc_pose_t array
	BioCon.setup(Middle_Mid[0]);
	// Servo IDs default to a count of 1~N, where N is integer input for setup()
	// In this example, there are four servos named 1~4.
	// Servo IDs can be manually configured using the setId(index,id) function

	SerialUSB.print("Done with motion engine setup.\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop()
{
	SerialUSB.print("loop() started. Going to Ready Pose...");
	BioCon.loadPose(Middle_Mid);	// Load goal positions stored in Middle_Mid

	BioCon.interpolateSetup(1000);	// Define transition time to be 1000 ms and start the transition to the goal pose
	while(BioCon.interpolating())	// Loop while still interpolating
	{
		BioCon.interpolateStep();	// Calculate and update intermediate servo positions
	}

	while(1)
	{
		char keypress = 0;
		if (SerialUSB.available())
			keypress = SerialUSB.read();

		if ((keypress == 'p') || (keypress == 'P'))
		{
			// Check that there is no sequence already running
			if (!BioCon.playing())
			{
				SerialUSB.print("\nStarting 'Sweeper' sequence...");
				// Load a sequence and start playing it
				BioCon.playSeq(Sweeper);
			}
		}
		else if ((keypress == 's') || (keypress == 'S'))
		{
			// Check that there is a sequence running so we do not flood
			//  the terminal with text
			if (BioCon.playing())
			{
				SerialUSB.print("\nStopping 'Sweeper' sequence...");
				BioCon.playing(false);	// 
			}
		}


		if (BioCon.playing())	// If still playing a sequence
		{
			// play() checks whether a new pose needs to be loaded.
			//  If a pose transition is in progress, then it will simply
			//   call interpolateStep().
			//  If a pose transition has finished, then it will load the next
			//   pose in the sequence before calling interpolateSetup().
			//  If a sequence has finished, it returns immediately.
			// interpolateStep() will delay however many milliseconds remain 
			//  until the next time step is reached, when it will then set
			//  the next intermediate goal pose.
			BioCon.play();
		}
		
	}	
}
