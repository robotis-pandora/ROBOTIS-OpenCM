/*
 * Dynamixel3.cpp
 *
 *  Created on: 2013. 10. 11.
 *      Author: in2storm
 */

#include "Dynamixel3.h"
#include "timer.h"
#include "Arduino-compatibles.h"
#include "dxl3.h"

Dynamixel3 Dxl3; //[ROBOTIS] declare dynamixel instance in advance, it is used by sketch code.


Dynamixel3::Dynamixel3() {
	// TODO Auto-generated constructor stub

}

Dynamixel3::~Dynamixel3() {
	// TODO Auto-generated destructor stub
}

void Dynamixel3::begin(int baud) {
	uint32 Baudrate = 0;

	// initialize GPIO D20(PB6), D21(PB7) as DXL TX, RX respectively
	gpio_set_mode(PORT_DXL_TXD3, PIN_DXL_TXD3, GPIO_AF_OUTPUT_PP);  //USART3 TXD
	gpio_set_mode(PORT_DXL_RXD3, PIN_DXL_RXD3, GPIO_INPUT_FLOATING);//USART3 RXD
	gpio_set_mode(PORT_TXRX_DIRECTION3, PIN_TXRX_DIRECTION3, GPIO_OUTPUT_PP);//DXL3 Half Duplex Direction Pin

	gpio_write_bit(PORT_TXRX_DIRECTION3, PIN_TXRX_DIRECTION3, 0 );// RX Enable


	//Initialize USART 1 device
	usart_init(USART3);


	//Calculate baudrate, refer to ROBOTIS support page.
	Baudrate = 2000000 / (baud + 1);

	usart_set_baud_rate(USART3, STM32_PCLK1, Baudrate);
	nvic_irq_set_priority(USART3->irq_num, 0);//[ROBOTIS][ADD] 2013-04-10 set to priority 0
	usart_attach_interrupt(USART3, dxlInterruptUart3);
	usart_enable(USART3);
	delay(80);

	gbIsDynmixelUsed3 = 1;  //[ROBOTIS]2012-12-13 to notify end of using dynamixel SDK to uart.c
	clearBuffer256Uart3();
	mCommStatus= 0;
}


void Dynamixel3::end(void){
	gbBusUsed3 = 0; //[ROBOTIS]2012-12-13 to notify end of using dynamixel SDK to uart.c
	//will be removed by ROBOTIS,.LTD. there maybe not be used...
}


/*
 *  [ROBOTIS][ADD][START] 2013-04-09 support read and write on dxl bus
 * */

byte Dynamixel3::readRaw(void){

	return RxByteFromDXL3();
}
void Dynamixel3::writeRaw(uint8 value){

	DXL_TXD3

	TxByteToDXL3(value);

	DXL_RXD3

}
/*
 * @brief : if data coming from dxl bus, returns 1, or if not, returns 0.
 *
 */
byte Dynamixel3::available(void){

	return checkNewArrive3();
}

byte Dynamixel3::readByte(byte bID, byte bAddress){
	gbpParameter3[0] = bAddress;
	gbpParameter3[1] = 1;
	if(txrx_Packet3(bID, INST_READ, 2)){
		mCommStatus = 1;
		return(gbpRxBuffer3[5]);
	}
	else{
		mCommStatus = 0;
		return 0xff;
	}
}

word Dynamixel3::readWord(byte bID, byte bAddress){
	gbpParameter3[0] = bAddress;
	gbpParameter3[1] = 2;
	if(txrx_Packet3(bID, INST_READ, 2))
	{
		mCommStatus = 1;
		return( (((word)gbpRxBuffer3[6])<<8)+ gbpRxBuffer3[5] );
	}
	else{
		mCommStatus = 0;
		return 0xffff;
	}
}

byte Dynamixel3::writeByte(byte bID, byte bAddress, byte bData){
	gbpParameter3[0] = bAddress;
	gbpParameter3[1] = bData;
	mCommStatus = txrx_Packet3(bID, INST_WRITE, 2);
	return mCommStatus;
}

byte Dynamixel3::writeWord(byte bID, byte bAddress, word wData){
    gbpParameter3[0] = bAddress;
    gbpParameter3[1] = (byte)(wData&0xff);
    gbpParameter3[2] = (byte)((wData>>8)&0xff);
    mCommStatus = txrx_Packet3(bID, INST_WRITE, 3);
    return mCommStatus;
}
/*
 * @brief Sets the target position and speed of the specified servo
 * @author Made by Martin S. Mason(Professor @Mt. San Antonio College)
 * @change 2013-04-17 changed by ROBOTIS,.LTD.
 * */
byte Dynamixel3::setPosition(byte ServoID, int Position, int Speed){
	gbpParameter3[0] = (unsigned char)30;
	gbpParameter3[1] = (unsigned char)DXL_LOBYTE(Position);
	gbpParameter3[2] = (unsigned char)DXL_HIBYTE(Position);
	gbpParameter3[3] = (unsigned char)DXL_LOBYTE(Speed);
	gbpParameter3[4] = (unsigned char)DXL_HIBYTE(Speed);
    return(txrx_Packet3(ServoID, INST_WRITE, 5));
}

/*
 * @Depreciated, use DXL_LOBYTE(w) instead of getLowByte() method
 * */
byte Dynamixel3::getLowByte( word wData ){

	return (byte)(wData & 0xff);
}
/*
 * @Depreciated, use DXL_HIBYTE(w) instead of getHighByte() method
 * */
byte Dynamixel3::getHighByte( word wData ){

	return ((wData & 0xff00) >> 8);
}
/*
 * @Depreciated, use DXL_MAKEWORD(w) instead of makeWord() method
 * */
uint16  Dynamixel3::makeWord( byte lowbyte, byte highbyte ){
	uint16 wData;

	wData = ((highbyte & 0x00ff) << 8);
	return (wData + lowbyte);
}

byte  Dynamixel3::ping( byte  bID ){
	return(txrx_Packet3(bID, INST_PING, 0));
}
byte  Dynamixel3::reset( byte  bID ){

	return(txrx_Packet3(bID, INST_RESET, 0));
}

byte  Dynamixel3::getResult(void){
	return mCommStatus;
}
/*
 * @brief initialize parameter and get ID, instruction for making packet
 * @start_addr : start address for sync write
 * @num_of_data : DATA1, DATA2.. -> number of data
 * @Param : packet parameter array for controlling all dxls in bus { ID1, DATA1, DATA2,..., ID2, DATA1, DATA2,..., ID3, DATA1, DATA2,... }
 * @array_length : actual length of parameter array
 */
byte Dynamixel3::syncWrite(byte start_addr, byte num_of_data, byte *param, int array_length){
	int i;
	mbLengthForPacketMaking = 0;
	mbIDForPacketMaking = BROADCAST_ID;
	mbInstructionForPacketMaking = INST_SYNC_WRITE;
	mCommStatus = 0;
	gbpParameter3[mbLengthForPacketMaking++] = start_addr;
	gbpParameter3[mbLengthForPacketMaking++] = num_of_data; // actual number of data as byte
	for(i=0; i < array_length; i++){
		gbpParameter3[mbLengthForPacketMaking++] = param[i];
	}
	mCommStatus = txrx_Packet3(mbIDForPacketMaking, mbInstructionForPacketMaking, mbLengthForPacketMaking);
	return mCommStatus;
}
byte Dynamixel3::syncWrite(byte start_addr, byte num_of_data, int *param, int array_length){
	int i,j,k,num;
	k=0;
	num = array_length/(1+num_of_data); //ID+DATA1+DATA2..
	mbLengthForPacketMaking = 0;
	mbIDForPacketMaking = BROADCAST_ID;
	mbInstructionForPacketMaking = INST_SYNC_WRITE;
	mCommStatus = 0;
	gbpParameter3[mbLengthForPacketMaking++] = start_addr;
	gbpParameter3[mbLengthForPacketMaking++] = num_of_data*2;
	for(i=mbLengthForPacketMaking; i < num*(1+num_of_data*2); i+=(1+num_of_data*2)){
		gbpParameter3[i] = param[k++]; //ID
		for(j=0; j < (num_of_data*2); j+=2){
			gbpParameter3[i+j+1] = (unsigned char)DXL_LOBYTE(param[k]); //low byte
			gbpParameter3[i+j+2] = (unsigned char)DXL_HIBYTE(param[k]);; //high byte
			k++;
		}
	}
	mbLengthForPacketMaking= i;
	mCommStatus = txrx_Packet3(mbIDForPacketMaking, mbInstructionForPacketMaking, mbLengthForPacketMaking);
	return mCommStatus;
}

/*
 * @brief initialize parameter and get ID, instruction for making packet
 * */
void Dynamixel3::initPacket(byte bID, byte bInst){
	mbLengthForPacketMaking = 0;
	mbIDForPacketMaking = bID;
	mbInstructionForPacketMaking = bInst;
	mCommStatus = 0;
}
/*
 * @brief just push parameters, individual ID or moving data, individual data length
 * */
void Dynamixel3::pushByte(byte value){
	//packet length is not above the maximum 143 bytes because size of buffer receiver has only 143 bytes capacity.
	//please refer to ROBOTIS e-manual (support.robotis.com)
	if(mbLengthForPacketMaking > 140)//prevent violation of memory access
		return;
	gbpParameter3[mbLengthForPacketMaking++] = value;
}
void Dynamixel3::pushParam(byte value){
	if(mbLengthForPacketMaking > 140)//prevent violation of memory access
			return;
	gbpParameter3[mbLengthForPacketMaking++] = value;
}
void Dynamixel3::pushParam(int value){

	if(mbLengthForPacketMaking > 140)//prevent violation of memory access
			return;
	gbpParameter3[mbLengthForPacketMaking++] = (unsigned char)DXL_LOBYTE(value);
	gbpParameter3[mbLengthForPacketMaking++] = (unsigned char)DXL_HIBYTE(value);
}
/*
 * @brief transfers packets to dynamixel bus
 * */
byte Dynamixel3::flushPacket(void){

	//TxDString("\r\n");
	//TxD_Dec_U8(gbLengthForPacketMaking);
	mCommStatus = txrx_Packet3(mbIDForPacketMaking, mbInstructionForPacketMaking, mbLengthForPacketMaking);
	return mCommStatus;
}
/*
 * @brief return current the total packet length
 * */
byte Dynamixel3::getPacketLength(void){
	return mbLengthForPacketMaking;
}



void Dynamixel3::setTxPacketId(byte id){
	mbIDForPacketMaking = id;

}
void Dynamixel3::setTxPacketInstruction(byte instruction){
	mbInstructionForPacketMaking = instruction;

}
void Dynamixel3::setTxPacketParameter( byte index, byte value ){
	gbpParameter3[index] = value;

}
void Dynamixel3::setTxPacketLength( byte length ){
	mbLengthForPacketMaking = length;

}
byte Dynamixel3::txrxPacket(void){
	mCommStatus = txrx_Packet3(mbIDForPacketMaking, mbInstructionForPacketMaking, mbLengthForPacketMaking);
	return mCommStatus;
}

int Dynamixel3::getRxPacketParameter( int index ){
	//return dxl_get_rxpacket_parameter( index );
	return gbpRxBuffer3[5 + index];
}
int Dynamixel3::getRxPacketLength(void){
	//return dxl_get_rxpacket_length();
	return gbpRxBuffer3[3]; //length index is 3 in status packet
}
/*
 *  ERROR Bit table is below.
#define ERRBIT_VOLTAGE		(1)
#define ERRBIT_ANGLE		(2)
#define ERRBIT_OVERHEAT		(4)
#define ERRBIT_RANGE		(8)
#define ERRBIT_CHECKSUM		(16)
#define ERRBIT_OVERLOAD		(32)
#define ERRBIT_INSTRUCTION	(64)
 * */
int Dynamixel3::getRxPacketError( byte errbit ){
	//return dxl_get_rxpacket_error( errbit );
	if( gbpRxBuffer3[4] & errbit ){ //error bit index is 4 in status packet
		return 1;
	}
	return 0;

}
