
/************************* (C) COPYRIGHT 2010 ROBOTIS *************************
* File Name          : dynamixel.h
* Author             : danceww
* Version            : V0.1
* Date               : 2010/08/26
* Description        : This file contains the dynamixel communication function.
                       for the firmware.
*******************************************************************************/
/* Copyright (C) 2013 ROBOTIS, Co., Ltd.
 *
 * @File: dxl.h ( <- dynamixel.h )
 * @Brief : ported dynamixel SDK from CM-530 firmware for CM-9XX series
 *
 */
#ifndef DXL_H_
#define DXL_H_


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "libpandora_types.h"
#include "gpio.h"
#include "usart.h"
#include "usb_type.h"
#include "dxl_constants.h"

/* Debug defines ------------------------------------------------------------*/
#ifdef CM9_DEBUG
//#define PRINT_OUT_PACKET_TO_USART2
//#define PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
//#define PRINT_OUT_TRACE_ERROR_PRINT_TO_USART2
#endif

/* Exported types ------------------------------------------------------------*/




/*
 * defines for error message
 * */
#define ERRBIT_VOLTAGE		(1)
#define ERRBIT_ANGLE		(2)
#define ERRBIT_OVERHEAT		(4)
#define ERRBIT_RANGE		(8)
#define ERRBIT_CHECKSUM		(16)
#define ERRBIT_OVERLOAD		(32)
#define ERRBIT_INSTRUCTION	(64)


/*Global Variables ----------------------------------------------------------------------------------------------*/
volatile byte  gbDXLWritePointer;
volatile byte  gbDXLReadPointer;
volatile byte  gbpDXLDataBuffer[256];

volatile byte gbpParameter[130+10];
volatile byte gbBusUsed;
uint32 gbRxLength;
byte gbpRxBuffer[255];
byte gbpTxBuffer[255];

uint8 gbIsDynmixelUsed; //[ROBOTIS]2012-12-13

/*
 * Raw methods for basic packet methods
 * */
void clearBuffer256(void);
byte checkNewArrive(void);
void TxByteToDXL( byte bTxData);
byte RxByteFromDXL(void);

/*
 * Basic packet communications methods
 * */
byte txrx_Packet(byte bID, byte bInst, byte bTxParaLen);
byte tx_Packet(byte bID, byte bInstruction, byte bParameterLength);
byte rx_Packet(byte bLength);
/*
 *
 */
/* for delay */
uint32 Dummy(uint32 tmp);
void uDelay(uint32 uTime);
void nDelay(uint32 nTime);

#ifdef __cplusplus
}
#endif


#endif /* DXL_H_ */
