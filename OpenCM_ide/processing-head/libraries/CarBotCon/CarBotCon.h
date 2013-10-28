/*
 *******************************************************************************
 *  Carbot Controller
 *******************************************************************************
 *  Input is a forward speed and a turning rate.
 *    Forward: 0~100
 *    Backward: -100~0
 *    Left: 0~100
 *    Right: -100~0
 *******************************************************************************
 *  LEGAL STUFF
 *******************************************************************************
 *  
 *  Copyright (c) 2013 Matthew Paulishen. All rights reserved.
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *******************************************************************************
 */
 
#ifndef _CARBOTCON_H_
#define _CARBOTCON_H_

#include "../HaViMo2/dxl_devices.h"
#include <Dynamixel.h>
#include <Arduino-compatibles.h>


#define MAX_SERVOS_PER_SIDE		4
#define SET_DIR_CW				(1<<10)
#define SET_DIR_CCW				(0<<10)

class CarBotCon
{
private:
	int left_n_, right_n_;
	int left_a_[MAX_SERVOS_PER_SIDE], right_a_[MAX_SERVOS_PER_SIDE];
	bool stopped_;

public:
	int SetRightServos(int, int*);
	int SetLeftServos(int, int*);
	int SetWheelMode(bool);

	int Go(int,int);
	int Stop(void);

	int SpinLeft(int);
	int SpinRight(int);
	int Forward(int);
	int Backward(int);
};


#endif
