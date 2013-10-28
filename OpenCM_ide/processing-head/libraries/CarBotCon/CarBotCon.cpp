/*
 *******************************************************************************
 *  Carbot Controller
 *******************************************************************************
 */
 
#include "CarBotCon.h"
/// Printing out to USB here can cause program to hang if not actually
///   listening to the SerialUSB output in the CM-9 IDE's terminal.
//#define VERBOSE_CARBOT_OUTPUT

int CarBotCon::SetLeftServos(int num, int * array)
{
	if (num > MAX_SERVOS_PER_SIDE)
		return -1;

	int iter;
	for (iter=0; iter<num; iter++)
	{
		left_a_[iter] = array[iter];
	}
	left_n_ = iter;
	return iter;
}

int CarBotCon::SetRightServos(int num, int * array)
{
	if (num > MAX_SERVOS_PER_SIDE)
		return -1;

	int iter;
	for (iter=0; iter<num; iter++)
	{
		right_a_[iter] = array[iter];
	}
	right_n_ = iter;
	return iter;
}

int CarBotCon::SetWheelMode(bool doot)
{
	int cW_limit, ccw_limit;
	int servoCount=0;

	if (doot)
	{
		cW_limit = 0;
		ccw_limit = 0;
	}
	else
	{
		cW_limit = 0;
		ccw_limit = 1023;
	}

	int iter;
	for (iter=0; iter<left_n_; iter++)
	{
		Dxl.writeWord(left_a_[iter], AXM_CW_ANGLE_LIMIT_L, cW_limit);
		Dxl.writeWord(left_a_[iter], AXM_CCW_ANGLE_LIMIT_L, ccw_limit);
		servoCount++;
	}
	for (iter=0; iter<right_n_; iter++)
	{
		Dxl.writeWord(right_a_[iter], AXM_CW_ANGLE_LIMIT_L, cW_limit);
		Dxl.writeWord(right_a_[iter], AXM_CCW_ANGLE_LIMIT_L, ccw_limit);
		servoCount++;
	}
	return servoCount;
}

int CarBotCon::Stop(void)
{
	if (!stopped_)
		return Go(0,0);
	else
		return 1;
}

int CarBotCon::Go(int vel, int rot)
{
	if ((vel>100) || (vel<-100))
		return -1;
	if ((rot>100) || (rot<-100))
		return -2;
	
	int ls=0, rs=0;
	// rot=80, vel=20
	// normalizer = abs(rot)+abs(vel) = 100
	// left: ((1023*80*CW)-(1023*20*CW))/normalizer  = (81840 - 20460) / 100 = 613.8 CW
	// right: ((1023*80*CW)+(1023*20*CW))/normalizer = (81840 + 20460) / 100 = 1023 CW

	// rot=80, vel=80
	// normalizer = abs(rot)+abs(vel) = 160
	// left: ((1023*80*CW)-(1023*80*CW))/normalizer  = (81840 - 81840) / 160 = 0 CW
	// right: ((1023*80*CW)+(1023*80*CW))/normalizer = (81840 + 81840) / 160 = 1023 CW

	// rot=-80, vel=20
	// normalizer = abs(rot)+abs(vel) = 100
	// left: ((1023*-80*CW)-(1023*20*CW))/normalizer  = (-81840 - 20460) / 100 = -1023 CW = 1023 CCW
	// right: ((1023*-80*CW)+(1023*20*CW))/normalizer = (-81840 + 20460) / 100 = -613.8 CW = 613.8 CCW

	// rot=0, vel=60
	// normalizer = abs(rot)+abs(vel) = 60 => 100
	// left: ((1023*0*CW)-(1023*60*CW))/normalizer  = (0 - 61380) / 100 = -613.8 CW = 613.8 CCW
	// right: ((1023*0*CW)+(1023*60*CW))/normalizer = (0 + 61380) / 100 = 613.8 CW

	// rot=0, vel=-60
	// normalizer = abs(rot)+abs(vel) = 60 => 100
	// left: ((1023*0*CW)-(1023*-60*CW))/normalizer  = (0 + 61380) / 100 = 613.8 CW 
	// right: ((1023*0*CW)+(1023*-60*CW))/normalizer = (0 - 61380) / 100 = -613.8 CW = 613.8 CCW

	if ( (rot!=0) || (vel!=0) )
	{
		int normalizer = abs(rot) + abs(vel);
		if (normalizer<100)
			normalizer = 100;

		ls = ( (1023*rot)-(1023*vel) ) / normalizer;
		rs = ( (1023*rot)+(1023*vel) ) / normalizer;
		
		if (ls < 0)
		{
			ls = ls*-1;
			ls = (ls&0x3FF)|SET_DIR_CCW;
		}
		else
		{
			ls = (ls&0x3FF)|SET_DIR_CW;
		}

		if (rs < 0)
		{
			rs = rs*-1;
			rs = (rs&0x3FF)|SET_DIR_CCW;
		}
		else
		{
			rs = (rs&0x3FF)|SET_DIR_CW;
		}
#ifdef VERBOSE_CARBOT_OUTPUT
		SerialUSB.print("Right-side: ");
		SerialUSB.println(rs);
		SerialUSB.print("Left-side: ");
		SerialUSB.println(ls);
#endif
	}

	int iter;
	for (iter=0; iter<left_n_; iter++)
	{
		Dxl.writeWord(left_a_[iter], AXM_MOVING_SPEED_L, ls);
	}
	for (iter=0; iter<right_n_; iter++)
	{
		Dxl.writeWord(right_a_[iter], AXM_MOVING_SPEED_L, rs);
	}
	if ( (ls>0) || (rs>0) )
		stopped_ = false;
	else
		stopped_ = true;

	return 1;
}



int CarBotCon::SpinLeft(int rot)
{
	if ((rot>100) || (rot<0))
		return -2;
	return Go(0, rot);
}

int CarBotCon::SpinRight(int rot)
{
	if ((rot>100) || (rot<0))
		return -2;
	return Go(0, -1*rot);
}

int CarBotCon::Forward(int vel)
{
	if ((vel>100) || (vel<0))
		return -1;
	return Go(vel, 0);

}

int CarBotCon::Backward(int vel)
{
	if ((vel>100) || (vel<0))
		return -1;
	return Go(-1*vel, 0);
}
