// Zigbee SDK platform independent header
#ifndef RC100_CORE_HEADER
#define RC100_CORE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "libpandora_types.h"
#include "usb_type.h"

#define PACKET_DATA0    		2
#define INVERSE_PACKET_DATA0 	3
#define PACKET_DATA1    		4
#define INVERSE_PACKET_DATA1 	5
#define PACKET_LENGTH 			6

volatile byte                   gbPacketWritePointer ;
volatile byte                   gbPacketReadPointer;
//volatile byte                   gbPacketPointer;
volatile byte                   gbpPacketDataBuffer[16+1+16];
volatile byte                   gbpPacket[PACKET_LENGTH+2];
volatile byte                   gbNewPacket;
volatile word                   gwZigbeeRxData;


int rc100_hal_tx( unsigned char *pPacket, int numPacket );
int rc100_hal_rx( unsigned char *pPacket, int numPacket );


///////////// device control methods ////////////////////////
int rc100Initialize( uint32 baudrate );
void rc100Terminate(void);
byte CheckNewArrive(void);
////////// communication methods ///////////////////////
int rc100TxData(int data);
int rc100RxCheck(void);
int rc100RxData(void);

byte RxDByteUart2(void);
void TxDByteUart2(byte bTxdData);

#ifdef __cplusplus
}
#endif

#endif
