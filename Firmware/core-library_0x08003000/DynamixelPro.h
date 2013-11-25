/*
 * DynamixelPro.h
 *
 *  Created on: 2013. 4. 25.
 *      Author: ROBOTIS,.LTD.
 */

#ifndef DYNAMIXELPRO_H_
#define DYNAMIXELPRO_H_


#include "libpandora_types.h"



typedef struct data {
    int             iID;
    int				iAddr;
    int             iLength;
    int             iError;
    byte  			iData[8];
} BulkData, *PBulkData;


class DynamixelPro {
private:
	byte mResult;
	byte usedDxlPro;
	BulkData mBulkData[32]; //Maximum dxl pro number is 32
public:
	DynamixelPro(int devNum);
	virtual ~DynamixelPro();


	/////////// Device control methods /////////////
	void begin(int buad);
	void end(void);  /**will be removed by ROBOTIS,.LTD. there maybe not be used...*/

	//// High communication methods ////////
	byte readByte(byte bID, int wAddress);
	byte writeByte(byte bID, int wAddress, byte bData);
	word readWord(byte bID, int wAddress);
	byte writeWord(byte bID, int bAddress, word wData);

	byte writeDword( byte bID, int wAddress, unsigned int value );
	uint32 readDword( byte bID, int wAddress );

	/*
	     * [ROBOTIS][ADD][START] 2013-04-09 support read and write on dxl bus
	     * */
	void writeRaw(byte value);
	byte readRaw(void);
	byte available(void);
    /*
     * New Methods for making a packet
     * you can make sync write packet and reg/action packet by using these methods
     */
	byte syncWrite(int start_addr, int data_length, byte *param, int param_length);
	byte syncWrite(int start_addr, int data_length, word *param, int param_length); //new
	byte syncWrite(int start_addr, int data_length, int *param, int param_length);//new
    /*void initSyncWrite(byte bID, byte bInst, int start_addr);
    void pushByte(byte value);
    void pushWord(word value);
    void pushDword(dword value);
    byte flushPacket(void);
    byte getPacketLength(void);*/

    /*
     * Dynamixel Pro Bulk Read & utility functions
     * */
    int bulkRead(byte *param, int param_length);
    int bulkRead(word *param, int param_length); //new
    //int bulkRead(int *param, int param_length); //new

    byte getBulkByte(int id, int addr);
    uint16 getBulkWord(int id, int addr);
    int getBulkDword(int id,int addr);

    int bulkWrite(byte *param, int param_length);

    byte getResult(void);

    /*
     * Dynamixel Pro Utility functions
     * */
//    int systemWrite(int id, int address, int length, unsigned char *data);
//    int factoryReset(int id, int option);
//    int reboot(int id);
};


#endif /* DYNAMIXELPRO_H_ */
