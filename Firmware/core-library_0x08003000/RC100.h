/*
 * RC100.h
 *
 *  Created on: 2013. 4. 24.
 *      Author: ROBOTIS,.LTD.
 */

#ifndef RC100_H_
#define RC100_H_

////////// define RC-100 button key value ////////////////
#define RC100_BTN_U		(1)
#define RC100_BTN_D		(2)
#define RC100_BTN_L		(4)
#define RC100_BTN_R		(8)
#define RC100_BTN_1		(16)
#define RC100_BTN_2		(32)
#define RC100_BTN_3		(64)
#define RC100_BTN_4		(128)
#define RC100_BTN_5		(256)
#define RC100_BTN_6		(512)

#include "libpandora_types.h"

class RC100 {
public:
	RC100();
	virtual ~RC100();

	int begin(void);
	void end(void);
	int writeData(int data);
	int available(void);
	int readData(void);

	void writeRaw(byte temp);
	byte readRaw(void);
};


#endif /* RC100_H_ */
