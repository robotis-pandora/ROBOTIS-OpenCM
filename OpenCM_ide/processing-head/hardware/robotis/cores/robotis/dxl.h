/*
 * dxl2.h
 *
 *  Created on: 2013. 11. 8.
 *      Author: in2storm
 */

#ifndef DXL2_H_
#define DXL2_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "usart.h"
#include "gpio.h"
#include "dxl_constants.h"


/** DYNAMIXEL device type  */
#define DXL_RX_BUF_SIZE 256
#define DXL_PARAMETER_BUF_SIZE 128

typedef struct dxl_dev {
/*

	gpio_dev *tx_port;      *< Maple pin's GPIO device
	gpio_dev *rx_port;      *< Maple pin's GPIO device
	gpio_dev *dir_port;      *< Maple pin's GPIO device

	uint8 tx_pin;
	uint8 rx_pin;
	uint8 dir_pin;*/
	uint8 write_pointer;
	uint8 read_pointer;
	uint8 data_buffer[DXL_RX_BUF_SIZE];
	voidFuncPtrUart handlers;
} dxl_dev;
extern dxl_dev *DXL_DEV1;
extern dxl_dev *DXL_DEV3;


void dxlInterrupt2(byte data);
void dxlInterrupt3(byte data);


void PrintBuffer(byte *bpPrintBuffer, byte bLength);

#ifdef __cplusplus
}
#endif
#endif /* DXL2_H_ */
