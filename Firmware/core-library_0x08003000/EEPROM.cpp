/*
 * EEPROM.cpp
 *
 *  Created on: 2013. 6. 12.
 *      Author: in2storm[ROBOTIS,.LTD.]
 */

#include "EEPROM.h"
#include "eeprom_core.h"

EEPROM::EEPROM() {
	// TODO Auto-generated constructor stub

}

EEPROM::~EEPROM() {
	// TODO Auto-generated destructor stub
}

void EEPROM::begin(){
	EEPROM_INIT();
}
int EEPROM::read(int vAddress){
	return (int)EEPROM_Read((uint16)vAddress);
}
void EEPROM::write(int vAddress, int data){
	EEPROM_Write((uint16)vAddress,(uint16)data);
}
void EEPROM::erase(int page){
	EEPROM_ERASE(page);
}
void EEPROM::eraseAll(void){
	EEPROM_ERASE(0);
	EEPROM_ERASE(1);
}
