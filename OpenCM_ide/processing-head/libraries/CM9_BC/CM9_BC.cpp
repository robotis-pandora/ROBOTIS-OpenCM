/*
  BioloidController.cpp - ArbotiX Library for Bioloid Pose Engine
  Copyright (c) 2008-2012 Michael E. Ferguson.  All right reserved.

  Copyright (c) 2012, 2013 Matthew Paulishen. Copypaster: NaN a.k.a. tician

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "CM9_BC.h"

#ifndef AXM_GOAL_POSITION_L
#define AXM_GOAL_POSITION_L			30
#endif
#ifndef AXM_PRESENT_POSITION_L
#define AXM_PRESENT_POSITION_L		36
#endif

/* new-style setup */
void BioloidController::setup(int servo_cnt)
{
	int i;
	// setup storage
	id_ = (unsigned char *) malloc(servo_cnt * sizeof(unsigned char));
	pose_ = (unsigned int *) malloc(servo_cnt * sizeof(unsigned int));
	nextpose_ = (unsigned int *) malloc(servo_cnt * sizeof(unsigned int));
	speed_ = (int *) malloc(servo_cnt * sizeof(int));
	// initialize
	poseSize = servo_cnt;
	for(i=0;i<poseSize;i++)
	{
		id_[i] = i+1;
		pose_[i] = 512 << BIOLOID_SHIFT;
		nextpose_[i] = 512 << BIOLOID_SHIFT;
	}
	interpolating = 0;
	playing = 0;
	lastframe_ = millis();
}
void BioloidController::setId(int index, int id)
{
	id_[index] = id;
}
int BioloidController::getId(int index)
{
	return id_[index];
}

/* load a named pose from FLASH into nextpose. */
void BioloidController::loadPose( unsigned int * addr )
{
	int i;
	poseSize = addr[0]; // number of servos in this pose
//	SerialUSB.print("\nLoading Pose");
	for(i=0; i<poseSize; i++)
	{
		nextpose_[i] = addr[1+i] << BIOLOID_SHIFT;
//		SerialUSB.print("\n  Servo ID=");SerialUSB.print(id_[i]);
//		SerialUSB.print(" to goal=");SerialUSB.print(nextpose_[i]);
//		SerialUSB.print(".  Current position=");SerialUSB.println(pose_[i]);
	}
}
/* read in current servo positions to the pose. */
void BioloidController::readPose()
{
//	SerialUSB.print("\nCurrent Pose");
	for(int i=0;i<poseSize;i++)
	{
		int temp = Dxl.readWord(id_[i],AXM_PRESENT_POSITION_L);
		if ((temp < 1024) && (temp > 0))
			pose_[i] = temp<<BIOLOID_SHIFT;
		else
			pose_[i] = 512<<BIOLOID_SHIFT;
//		SerialUSB.print("\n  Servo ID=");SerialUSB.print(id_[i]);
//		SerialUSB.print(" at present=");SerialUSB.println(pose_[i]);
//		delay(25);
	}
}
/* write pose out to servos using sync write. */
void BioloidController::writePose()
{
	int temp;
	int numParams = 2 + (poseSize * 3);   // 3 = id + GOAL_L + GOAL_H

	Dxl.setTxPacketId( BROADCAST_ID );
	Dxl.setTxPacketInstruction( INST_SYNC_WRITE );
	Dxl.setTxPacketLength( numParams );
	Dxl.setTxPacketParameter( 0, AXM_GOAL_POSITION_L );
	Dxl.setTxPacketParameter( 1, 2 );	// writing two bytes
//	SerialUSB.print("\nNext Pose Packet");
	for(int i=0; i<poseSize; i++)
	{
		temp = pose_[i] >> BIOLOID_SHIFT;
//		SerialUSB.print("\n  Servo ID=");SerialUSB.print(id_[i]);
//		SerialUSB.print(" to goal=");SerialUSB.println(temp);
		Dxl.setTxPacketParameter( 2 + 3*i + 0, id_[i] );
		Dxl.setTxPacketParameter( 2 + 3*i + 1, (temp)&0xff );
		Dxl.setTxPacketParameter( 2 + 3*i + 2, (temp>>8)&0xff );
	}
	Dxl.txrxPacket();
}

/* set up for an interpolation from pose to nextpose over TIME 
	milliseconds by setting servo speeds. */
void BioloidController::interpolateSetup(int time)
{
	int i;
	int frames = (time/BIOLOID_FRAME_LENGTH) + 1;
	lastframe_ = millis();
	readPose();
	// set speed each servo...
	for(i=0;i<poseSize;i++)
	{
		if(nextpose_[i] > pose_[i])
		{
			speed_[i] = (nextpose_[i] - pose_[i])/frames + 1;
		}
		else
		{
			speed_[i] = (pose_[i]-nextpose_[i])/frames + 1;
		}
	}
	interpolating = 1;
}
/* interpolate our pose, this should be called at about 30Hz. */
void BioloidController::interpolateStep()
{
	if(interpolating == 0) return;
	int i;
	int complete = poseSize;
	while(millis() - lastframe_ < BIOLOID_FRAME_LENGTH);
	lastframe_ = millis();
//	readPose();
	// update each servo
	for(i=0;i<poseSize;i++)
	{
		int diff = nextpose_[i] - pose_[i];
		if(diff == 0)
		{
			complete--;
		}
		else
		{
			if(diff > 0)
			{
				if(diff < speed_[i])
				{
					pose_[i] = nextpose_[i];
					complete--;
				}
				else
				{
					pose_[i] += speed_[i];
				}
			}
			else
			{
				if((-diff) < speed_[i])
				{
					pose_[i] = nextpose_[i];
					complete--;
				}
				else
				{
					pose_[i] -= speed_[i];
				}
			}
		}
	}
	if(complete <= 0)
	{
		interpolating = 0;
	}
	writePose();
}

/* get a servo value in the current pose */
int BioloidController::getCurPose(int id)
{
	for(int i=0; i<poseSize; i++)
	{
		if( id_[i] == id )
			return ((pose_[i]) >> BIOLOID_SHIFT);
	}
	return -1;
}
/* get a servo value in the next pose */
int BioloidController::getNextPose(int id)
{
	for(int i=0; i<poseSize; i++)
	{
		if( id_[i] == id )
			return ((nextpose_[i]) >> BIOLOID_SHIFT);
	}
	return -1;
}
/* set a servo value in the next pose */
void BioloidController::setNextPose(int id, int pos)
{
	for(int i=0; i<poseSize; i++)
	{
		if( id_[i] == id )
		{
			nextpose_[i] = (pos << BIOLOID_SHIFT);
			return;
		}
	}
}

/* play a sequence. */
void BioloidController::playSeq( transition_t * addr )
{
	sequence = (transition_t *) addr;
	// number of transitions left to load
	transitions = ( sequence->time );
//	SerialUSB.print("Number Transitions: ");SerialUSB.println(transitions);
	sequence++;
	// load a transition
	loadPose( sequence->pose );
	interpolateSetup( sequence->time );
	transitions--;
	playing = 1;
}
/* keep playing our sequence */
void BioloidController::play()
{
	if(playing == 0)
		return;
	if(interpolating > 0)
	{
		interpolateStep();
	}
	else  // move onto next pose
	{
		sequence++;
		if(transitions > 0)
		{
			loadPose( sequence->pose );
			interpolateSetup( sequence->time );
			transitions--;
		}
		else
		{
			playing = 0;
		}
	}
}

