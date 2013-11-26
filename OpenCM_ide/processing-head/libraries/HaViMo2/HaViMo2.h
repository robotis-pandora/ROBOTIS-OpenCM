/*
 *******************************************************************************
 *  HaViMo2.h - 2013-04-17
 *******************************************************************************
 *  A header file of questionable quality for using the HaViMo2
 *  
 *******************************************************************************
 *  LEGAL STUFF
 *******************************************************************************
 *  
 *   'Dynamixel' is property of Robotis, Inc.
 *      http://www.robotis.com
 *  
 *  Copyright (c) 2011, 2012, 2013 Matthew Paulishen. All rights reserved.
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
 
#ifndef _HAVIMO2_H_
#define _HAVIMO2_H_
#include "dxl_devices.h"
//#include "wirish.h"
#include <Dynamixel.h>

//extern Dynamixel *pDxl;
// HaViMo2
#define HaViMo2_ID                      100
typedef enum {
	Unknown                           = 0,
	Black                             = 0,
	Ball                              = 1,
	Teal                              = 1,
	Field                             = 2,
	Red                               = 2,
	MyGoal                            = 3,
	Green                             = 3,
	OppGoal                           = 4,
	Purple                            = 4,
	Robot                             = 5,
	White                             = 5,
	Cyan                              = 6,
	Magenta                           = 7
} HaViMo2_Color_t;

class HaViMo2_Controller
{
public:

private:
	typedef struct {
		uint8_t     Index, Color;
		uint16_t    NumPix;
		uint32_t    SumX, SumY;
		uint8_t     MaxX, MinX, MaxY, MinY;
	} HaViMo2_Region_t;

	typedef struct {
		uint8_t             valid;
		HaViMo2_Region_t    rb[15];
	} HaViMo2_Region_Buffer_t;

	HaViMo2_Region_Buffer_t hvm2rb;

public:
	/**
	 * Function to begin an image capture with the HaViMo2 camera module.
	 * @see ready() and recover()
	 */
	void capture(void);
	/**
	 * Function to check if HaViMo2 has finished capturing image.
	 * @see capture() and recover()
	 */
	bool ready(void);
	/**
	 * Function to retrieve an image buffer from a HaViMo2 camera module.
	 * @see capture() and ready()
	 * @return The number of valid regions found in the image.
	 */
	uint8_t recover(void);

	/**
	 * Function to retrieve number of valid regions in buffer.
	 * @see recover()
	 * @return The number of valid regions found in the image.
	 */
	uint8_t regions(void);
	/**
	 * Function to retrieve the color of this region index.
	 * @see size() and avgX() and avgY()
	 * @return The color of the valid region, else 0xFF.
	 */
	uint8_t color(uint8_t region_index);
	/**
	 * Function to retrieve the number of pixels in this region index.
	 * @see color() and avgX() and avgY()
	 * @return The size of the valid region, else 0.
	 */
	uint16_t size(uint8_t region_index);
	/**
	 * Function to X-centroid of the color of this region index.
	 * @see color() and size() and and avgY()
	 * @return The X-location of the valid region center, else 0.
	 */
	uint8_t avgX(uint8_t region_index);
	/**
	 * Function to Y-centroid of the color of this region index.
	 * @see color() and size() and and avgX()
	 * @return The Y-location of the valid region center, else 0.
	 */
	uint8_t avgY(uint8_t region_index);
};

#endif
