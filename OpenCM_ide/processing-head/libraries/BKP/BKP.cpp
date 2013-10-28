/*
 * BKP.cpp
 *
 *  Created on: 2013. 6. 12.
 *      Author: ROBOTIS,.LTD.
 */

#include "BKP.h"
#include "bkp_core.h"


BKP::BKP() {
	// TODO Auto-generated constructor stub

}

BKP::~BKP() {
	// TODO Auto-generated destructor stub
}

void BKP::begin(){
	bkp_init();
}
void BKP::enable(void){
	bkp_enable_writes();

}
void BKP::disable(void){
	bkp_disable_writes();

}
void BKP::write(uint8 reg, int wData){
	bkp_write(reg, (uint16)wData);
}
void BKP::write(uint8 reg, uint16 wData){
	bkp_write(reg, wData);
}
void BKP::write(uint8 reg, uint8 bData){
	bkp_write(reg, (uint16)bData);
}
uint16 BKP::read(uint8 reg){

	return bkp_read(reg);
}




