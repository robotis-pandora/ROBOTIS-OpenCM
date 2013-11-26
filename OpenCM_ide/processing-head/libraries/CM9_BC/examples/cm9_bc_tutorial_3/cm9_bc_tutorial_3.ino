#include <CM9_BC.h>
Dynamixel Dxl(3);
Dynamixel *pDxl = &Dxl;
int Offset_Calibration[] __FLASH__ = {4,0,0,0,0};

bc_pose_t ServoIDs[] __FLASH__ = {4,1,2,3,4};


bc_pose_t ReadyPose[] __FLASH__ = {4,512,512,512,512};

bc_pose_t Middle_Max[] __FLASH__ = {4,768,768,768,768};
bc_pose_t Middle_Min[] __FLASH__ = {4,256,256,256,256};

bc_pose_t Alter_2[] __FLASH__ = {4,768,768,256,256};
bc_pose_t Alter_3[] __FLASH__ = {4,256,256,768,768};


bc_seq_t ReadyPage[] __FLASH__ = {{ServoIDs, 1}, {ReadyPose, 1000}};

bc_seq_t Sweeper[] __FLASH__ = {{ServoIDs,4}, {Middle_Max,500}, {ReadyPose,500}, {Middle_Min,500}, {ReadyPose,500}};

bc_seq_t Alternator_1[] __FLASH__ = {{ServoIDs,2}, {Alter_2,500}, {ReadyPose,500}};
bc_seq_t Alternator_2[] __FLASH__ = {{ServoIDs,2}, {Alter_3,500}, {ReadyPose,500}};


rpm_page_t Tutorial_RoboPlusMotion_Array[] __FLASH__ = {
	{0,					0,		4},	// 0
	{ReadyPage,			0,		0},	// 1
	{Sweeper,			2,		0},	// 2
	{Alternator_1,		4,		0},	// 3
	{Alternator_2,		3,		0}	// 4
};

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

	// Load the RoboPlusMotion_Array from our _RPM header file
	BioCon.RPM_Setup(Tutorial_RoboPlusMotion_Array);
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
	BioCon.MotionPage(1);	// ReadyPage
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
		char keypress = 0;
		if (SerialUSB.available())
		{
			keypress = SerialUSB.read();
		}
		
		if (keypress == '1')
		{
			BioCon.MotionPage(1);	// ReadyPage
		}
		else if (keypress == '2')
		{
			BioCon.MotionPage(2);	// Sweeper
		}
		else if (keypress == '3')
		{
			BioCon.MotionPage(3);	// Alternator_1
		}
		else if (keypress == '4')
		{
			BioCon.MotionPage(4);	// Alternator_2
		}
		else if (keypress > 0)
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
