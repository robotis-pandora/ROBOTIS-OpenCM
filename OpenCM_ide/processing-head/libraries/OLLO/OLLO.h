/*
 * OLLO.h For CM-904(new CM9 Series board)
 *
 *  Created on: 2013. 5. 30.
 *      Author: ROBOTIS,.LTD.
 */

#ifndef OLLO_H_
#define OLLO_H_
#include "libpandora_types.h"

enum OLLO_DEVICE_INDEX {
    IR_SENSOR,
    TOUCH_SENSOR,
    GYRO_SENOSR,
    DMS_SENSOR,
    PIR_SENSOR,
    MAGNETIC_SENSOR,
    COLOR_SENSOR,
    ULTRASONIC_SENSOR,
    LED_DISPLAY
};
enum COLOR_INDEX {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    ORANGE,
    BLACK,
    WHITE
};
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_BLUE 3

class OLLO {
private:
	uint8 mMot_plus;
	uint8 mMot_minus;
public:
	OLLO();
	virtual ~OLLO();
	//General 3PIN sensors
	void begin(int devNum);
	void begin(int devNum, int device_index);
	void begin(int devNum, int device_index, voidFuncPtr handler);

	int read(int devNum);
	int read(int devNum, int device_index);
	int read(int devNum, int device_index, int sub_index);

	void write(int devNum, uint8 leftVal,  uint8 rightVal);
	void write(int devNum, uint8 leftVal, uint8 centerVal, uint8 rightVal);

	//IR Sensor Module
	//void beginIR(int devNum);
	//LED Module
	void writeLED(int devNum,uint8 leftVal, uint8 rightVal );
	//Button Module
	//void beginButton(int devNum,voidFuncPtr handler);
	//int readColor(int devNum, int colorIndex);
	void setColor(int colorIndex);
};

#endif /* OLLO_H_ */
