/*
 * dxl.c
 *
 *  Created on: 2013. 11. 8.
 *      Author: in2storm
 */

#include "dxl.h"


static dxl_dev dxl1 = {
		.write_pointer = 0,
		.read_pointer = 0,
		.handlers = dxlInterrupt2

};
static dxl_dev dxl3 = {
		.write_pointer = 0,
		.read_pointer = 0,
		.handlers = dxlInterrupt3

};
dxl_dev *DXL_DEV1 = &dxl1;
dxl_dev *DXL_DEV3 = &dxl3;


void dxlInterrupt2(byte data){
	//gbpDXLDataBuffer[gbDXLWritePointer++ & 0xFF] = (uint8)USART1->regs->DR; //[ROBOTIS]Support to Dynamixel SDK.
	//TxDStringC("test");
	DXL_DEV1->data_buffer[DXL_DEV1->write_pointer++ & 0xFF] = data;
}
void dxlInterrupt3(byte data){
	//gbpDXLDataBuffer[gbDXLWritePointer++ & 0xFF] = (uint8)USART1->regs->DR; //[ROBOTIS]Support to Dynamixel SDK.
	//TxDHex8C(data);
	DXL_DEV3->data_buffer[DXL_DEV3->write_pointer++ & 0xFF] = data;
}
