
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

#ifndef DXL3_H_
#define DXL3_H_



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

#define TxD8Dec TxDHex8C
#define TXD_STRING(FPTR) TxDStringC(FPTR)
#define TxD8 TxDByteC
*/


/*Global Variables ----------------------------------------------------------------------------------------------*/
volatile byte  gbDXLWritePointer3;
volatile byte  gbDXLReadPointer3;
volatile byte  gbpDXLDataBuffer3[256];

volatile byte gbpParameter3[130+10];
volatile byte gbBusUsed3;
uint32 gbRxLength3;
byte gbpRxBuffer3[255];
byte gbpTxBuffer3[255];

uint8 gbIsDynmixelUsed3; //[ROBOTIS]2012-12-13

/*
 * Raw methods for basic packet methods
 * */
void clearBuffer256Uart3(void);
byte checkNewArrive3(void);
void TxByteToDXL3( byte bTxData);
byte RxByteFromDXL3(void);


/*
 * Basic packet communications methods
 * */
byte txrx_Packet3(byte bID, byte bInst, byte bTxParaLen);
byte tx_Packet3(byte bID, byte bInstruction, byte bParameterLength);
byte rx_Packet3(byte bLength);
/*
 *
 */
/* for delay */
uint32 Dummy3(uint32 tmp);
void uDelay3(uint32 uTime);
void nDelay3(uint32 nTime);

void dxlInterruptUart3(byte data);

#ifdef __cplusplus
}
#endif


#endif /* DXL3_H_ */
