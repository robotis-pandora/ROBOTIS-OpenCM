/*
 * Dynamixel.h
 *
 *  Created on: 2013. 11. 8.
 *      Author: in2storm
 *      brief : 2013-11-12 [ROBOTIS] revised for OpenCM board
 */

#ifndef DYNAMIXEL_H_
#define DYNAMIXEL_H_

#include "dxl.h"


class Dynamixel {
public:
	Dynamixel(int dev_num);
	virtual ~Dynamixel();

	/////////// Device control methods /////////////
	void begin(int buad);
	uint8 readRaw(void);
	uint8 available(void);
	void writeRaw(uint8);

	byte txRxPacket(byte bID, byte bInst, byte bTxParaLen);
	byte txPacket(byte bID, byte bInstruction, byte bParameterLength);

	//// High communication methods ////////
	byte readByte(byte bID, byte bAddress);
	word readWord(byte bID, byte bAddress);
	byte writeByte(byte bID, byte bAddress, byte bData);
	byte writeWord(byte bID, byte bAddress, word wData);
	byte ping( byte  bID );
	byte reset( byte  bID );
	byte setPosition(byte ServoID, int Position, int Speed);
	byte getResult(void); // use getTxRxStatus() instead of getResult() method
	byte getTxRxStatus(void);// made by NaN (Robotsource.org)
	byte setLibStatusReturnLevel(byte); // made by NaN (Robotsource.org)
	byte setLibNumberTxRxAttempts(byte);// made by NaN (Robotsource.org)

	 /////// Methods for making a packet ////////
	void setTxPacketId( byte id );
	void setTxPacketInstruction( byte instruction );
	void setTxPacketParameter( byte index, byte value );
	void setTxPacketLength( byte length );
	byte txrxPacket(void);
	int getRxPacketParameter( int index );
	int getRxPacketLength(void);
	int getRxPacketError( byte errbit );
	/*
	 * New Methods for making a packet
	 * you can make sync write packet and reg/action packet by using these methods
	 */
	byte syncWrite(byte start_addr, byte num_of_data, byte *param, int array_length);
	byte syncWrite(byte start_addr, byte num_of_data, int *param, int array_length);//new

	/*
	 * New Methods for making a packet
	 */
	void initPacket(byte bID, byte bInst);
	void pushByte(byte value);
	void pushParam(int value);
	void pushParam(byte value);
	byte flushPacket(void);
	byte getPacketLength(void);

	/*
	 * Utility methods for Dynamixel
	 */

	byte getLowByte( word wData ); //can be replaced by DXL_LOBYTE(w)
	byte getHighByte( word wData );//can be replaced by DXL_HIBYTE(w)
	word makeWord( byte lowbyte, byte highbyte ); //can be replaced by DXL_MAKEWORD(w)

private:
	void printBuffer(byte *bpPrintBuffer, byte bLength);
	uint32 Dummy(uint32 tmp);
	void uDelay(uint32 uTime);
	void nDelay(uint32 nTime);
	void dxlTxEnable(void);
	void dxlTxDisable(void);
	void clearBuffer(void);
	byte rxPacket(byte bRxLength);

	uint8 setDxlLibStatRtnLvl(uint8); // inspired by NaN (robotsource.org)
	uint8 setDxlLibNumTries(uint8); // inspired by NaN (robotsource.org)

	dxl_dev *mDxlDevice;
	usart_dev *mDxlUsart;  /*< USART Device*/
	gpio_dev *mTxPort;
	gpio_dev *mRxPort;
	gpio_dev *mDirPort;
	uint8 mTxPin;
	uint8 mRxPin;
	uint8 mDirPin;
	uint8 mTxDEnablePin;  //CM-900 ES only
	uint8 mRxDEnablePin;  //CM-900 ES only
	uint8 mRxBuffer[DXL_RX_BUF_SIZE];
	uint8 mTxBuffer[DXL_RX_BUF_SIZE];
	uint8 mParamBuffer[DXL_PARAMETER_BUF_SIZE];
	uint8 mBusUsed;
	uint8 mRxLength;  // the length of the received data from dynamixel bus

	// additions to return proper COMM_* status
	uint8 mDXLtxrxStatus;  // inspired by NaN (robotsource.org)
	// additions to permit non-default Status Return Level settings without returning errors
	uint8 gbDXLStatusReturnLevel;
	// additions to adjust number of txrx attempts
	uint8 gbDXLNumberTxRxAttempts;

	byte mbLengthForPacketMaking;
	byte mbIDForPacketMaking;
	byte mbInstructionForPacketMaking;
	byte mCommStatus;

};


#endif /* DYNAMIXEL_H_ */
