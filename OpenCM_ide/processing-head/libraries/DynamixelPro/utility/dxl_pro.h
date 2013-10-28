/*
 * dxl_pro.h
 *
 *  Created on: 2013. 4. 25.
 *      Author: ROBOTIS,.LTD.
 */

#ifndef DXL_PRO_H_
#define DXL_PRO_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "libpandora_types.h"
#include "gpio.h"
#include "usart.h"
#include "usb_type.h"
#include "dxl_constants.h"
//#include "dxl.h"



/* Exported types ------------------------------------------------------------*/
enum PACKET_INDEX {
    PKT_HEADER0,
    PKT_HEADER1,
    PKT_HEADER2,
    PKT_RESERVED,
    PKT_ID,
    PKT_LENGTH_L,
    PKT_LENGTH_H,
    PKT_INSTRUCTION,
    PKT_PARAMETER
};


enum INSTRUCTION_EX {
	INST_PING_EX           = 1,
	INST_READ_EX           = 2,
	INST_WRITE_EX          = 3,
	INST_REG_WRITE_EX      = 4,
	INST_ACTION_EX         = 5,
	INST_FACTORY_RESET_EX  = 6,
	INST_REBOOT_EX         = 8,
	INST_SYSTEM_WRITE_EX   = 13,   // 0x0D
	INST_STATUS_EX         = 85,   // 0x55
	INST_SYNC_READ_EX      = 130,  // 0x82
	INST_SYNC_WRITE_EX     = 131,  // 0x83
	INST_BULK_READ_EX      = 146,  // 0x92
	INST_BULK_WRITE_EX     = 147   // 0x93
};
#define MAXNUM_TXPACKET     (255)//(65535)
#define MAXNUM_RXPACKET     (255)//(65535)


#define PING_STATUS_LENGTH  (14)
#define MAX_ID              (252)

#if 0 // move to original dxl codes(dxl_constants.h)
///////////////// utility for value ///////////////////////////
#define DXL_MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(((unsigned long)(a)) & 0xff)) | ((unsigned short)((unsigned char)(((unsigned long)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b)     ((unsigned int)(((unsigned short)(((unsigned long)(a)) & 0xffff)) | ((unsigned int)((unsigned short)(((unsigned long)(b)) & 0xffff))) << 16))
#define DXL_LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define DXL_HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define DXL_LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#define DXL_HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))
#endif

/*Global Variables ----------------------------------------------------------------------------------------------*/
volatile byte  gbDXLWritePointerEx;
volatile byte  gbDXLReadPointerEx;
volatile byte  gbpDXLDataBufferEx[1024];//[256]; //2013-08-05 changed 256 to 1024

volatile byte gbpParameterEx[255];//[130+10];  //2013-08-05 changed 130+10 to 255
volatile byte gbBusUsedEx;
uint32 gbRxLengthEx;
byte gbpRxBufferEx[MAXNUM_RXPACKET];
byte gbpTxBufferEx[MAXNUM_TXPACKET];


void dxlProInterrupt(byte data);
/*
 * Raw methods for basic packet methods
 * */
void clearBuffer256Ex(void);
byte checkNewArriveEx(void);
void TxByteToDXLEx( byte bTxData);
byte RxByteFromDXLEx(void);
#ifdef CM9_DEBUG
extern void TxDByteC(uint8 buf);
extern void TxDStringC(char *str);
extern void TxDHex8C(u16 bSentData);
extern void TxDHex16C(u16 wSentData);
extern void TxDHex32C(u32 lSentData);
void PrintBufferEx(byte *bpPrintBuffer, byte bLength);
#endif

/*
 * Basic packet communications methods
 * */
byte txrx_PacketEx(byte bID, byte bInst, word wTxParaLen);
byte tx_PacketEx(byte bID, byte bInst, word wParameterLength);
byte rx_PacketEx(word wLength);
/*
 * New functions for DXL Pro
 */
void dxl_add_stuffing(unsigned char * packet);
void dxl_remove_stuffing(unsigned char * packet);
unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);

uint32 dxl_get_baudrate(int baudnum);

#ifdef __cplusplus
}
#endif



#endif /* DXL_PRO_H_ */
