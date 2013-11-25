/*
 * RC100.cpp
 *
 *  Created on: 2013. 4. 24.
 *      Author: ROBOTIS,.LTD.
 */

#include "RC100.h"
#include "rc100_core.h"


RC100::RC100() {
	// TODO Auto-generated constructor stub

}

RC100::~RC100() {
	// TODO Auto-generated destructor stub

}

int RC100::begin(void){
	return rc100Initialize(57600);
}
void RC100::end(void){
	rc100Terminate();
}
int RC100::writeData(int data){
	return rc100TxData(data);
}
void RC100::writeRaw(byte temp){

	TxDByteUart2(temp);
}
byte RC100::readRaw(void){
	return RxDByteUart2();
}
int RC100::available(void){
	return rc100RxCheck();
}
int RC100::readData(void){
	return rc100RxData();
}
