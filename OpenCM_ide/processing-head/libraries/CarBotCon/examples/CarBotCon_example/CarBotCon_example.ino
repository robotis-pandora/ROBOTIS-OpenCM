/*
 *******************************************************************************
 *  CM-5 Carbot control with a RC-100
 *******************************************************************************
 */

#include <RC100.h>

RC100 Remo;
int RcvData = 0;

//#define VERBOSE_STEERING
#include <CarBotCon.h>
CarBotCon carbo;
unsigned int timeout_count;
int velo, radi;

void setup()
{
	Dxl.begin(1);
	SerialUSB.begin();
/*
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
*/
	SerialUSB.print("Now starting program\n");

	Remo.begin();
	int righties[] = {1,3};
	carbo.SetRightServos(2, righties);
	int lefties[] = {2,4};
	carbo.SetLeftServos(2, lefties);
	carbo.SetWheelMode(true);
	timeout_count = 1000;
	velo = 0; radi = 0;
}

void loop()
{
	while(1)
	{
		if(Remo.available())
		{
			RcvData = Remo.readData();

			if(RcvData & RC100_BTN_U)
			{
				carbo.Forward(100);
				timeout_count = 0;
#ifdef VERBOSE_STEERING
				SerialUSB.print("Forward\n");
#endif
			}
			else if (RcvData & RC100_BTN_D)
			{
				carbo.Backward(100);
				timeout_count = 0;
#ifdef VERBOSE_STEERING
				SerialUSB.print("Backward\n");
#endif
			}
			else if (RcvData & RC100_BTN_L)
			{
				carbo.SpinLeft(100);
				timeout_count = 0;
#ifdef VERBOSE_STEERING
				SerialUSB.print("SpinLeft\n");
#endif
			}
			else if (RcvData & RC100_BTN_R)
			{
				carbo.SpinRight(100);
				timeout_count = 0;
#ifdef VERBOSE_STEERING
				SerialUSB.print("SpinRight\n");
#endif
			}
			else if (RcvData & RC100_BTN_1)
			{
				velo+=10;
				if (velo>100)
					velo = 100;
				carbo.Go(velo, radi);
				timeout_count = 0;
			}
			else if (RcvData & RC100_BTN_2)
			{
				radi+=10;
				if (radi>100)
					radi = 100;
				carbo.Go(velo, radi);
				timeout_count = 0;
			}
			else if (RcvData & RC100_BTN_3)
			{
				velo-=10;
				if (velo<-100)
					velo = -100;
				carbo.Go(velo, radi);
				timeout_count = 0;
			}
			else if (RcvData & RC100_BTN_4)
			{
				radi-=10;
				if (radi<-100)
					radi = -100;
				carbo.Go(velo, radi);
				timeout_count = 0;
			}
			else if (RcvData & RC100_BTN_5)
			{
				carbo.Go(velo, radi);
				timeout_count = 0;
			}
			else
			{
				timeout_count = 1000;
			}
		}
		else
		{
			timeout_count++;
			delay(5);
		}

		if (timeout_count>50)
		{
			carbo.Stop();
#ifdef VERBOSE_STEERING
			SerialUSB.print("Stop\n");
#endif
		}

	}
}
