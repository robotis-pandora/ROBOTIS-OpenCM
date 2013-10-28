/*
 * BKP.h
 *
 *  Created on: 2013. 6. 12.
 *      Author: in2storm[ROBOTIS,.LTD.]
 *  CM9 Series board libraries for backup registers(BKP)
 *  BKP register is 16-bit registers for storing user application data
 */

#ifndef BKP_H_
#define BKP_H_

#include "libpandora_types.h"

class BKP {
public:
	BKP();
	virtual ~BKP();

	void begin(void);

	uint16 read(uint8 reg);


	void write(uint8 reg, uint16 wData);
	void write(uint8 reg, uint8 bData);
	void write(uint8 reg, int wData);

	void enable(void);
	void disable(void);

};


#endif /* BKP_H_ */
