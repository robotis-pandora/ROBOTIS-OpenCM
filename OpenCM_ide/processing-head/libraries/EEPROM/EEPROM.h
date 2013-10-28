/*
 * EEPROM.h
 *
 *  Created on: 2013. 6. 12.
 *      Author: in2storm[ROBOTIS,.LTD.]
 */

#ifndef EEPROM_H_
#define EEPROM_H_


class EEPROM {
public:
	EEPROM();
	virtual ~EEPROM();

	void begin();
	void write(int vAddress, int data);
	int read(int vAddress);
	void erase(int page);
	void eraseAll(void);

};

#endif /* EEPROM_H_ */
