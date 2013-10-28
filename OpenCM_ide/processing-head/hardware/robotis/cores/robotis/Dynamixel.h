/*
 * Dynamixel.h
 *
 *  Created on: 2013. 4. 18.
 *      Author: in2storm
 */

#ifndef DYNAMIXEL_H_
#define DYNAMIXEL_H_


#include "libpandora_types.h"
#include "dxl_constants.h"

class Dynamixel {
private:

	byte mbLengthForPacketMaking;
	byte mbIDForPacketMaking;
	byte mbInstructionForPacketMaking;
	byte mCommStatus;

public:
	 /**
	 * @brief Construct a new Dynamixel instance.
	 * @param -
	 */

	Dynamixel();
	virtual ~Dynamixel();

	/////////// Device control methods /////////////
	void begin(int buad);
	void end(void);  /**will be removed by ROBOTIS,.LTD. there maybe not be used...*/

    //// High communication methods ////////
	byte readByte(byte bID, byte bAddress);
	word readWord(byte bID, byte bAddress);
	byte writeByte(byte bID, byte bAddress, byte bData);
	byte writeWord(byte bID, byte bAddress, word wData);
	byte ping( byte  bID );
	byte reset( byte  bID );
	byte setPosition(byte ServoID, int Position, int Speed);
    byte getResult(void);


    /////// Methods for making a packet ////////
    void setTxPacketId( byte id );
    void setTxPacketInstruction( byte instruction );
    void setTxPacketParameter( byte index, byte value );
    void setTxPacketLength( byte length );
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
    uint16 makeWord( byte lowbyte, byte highbyte ); //can be replaced by DXL_MAKEWORD(w)

    ////////// packet communication methods ///////////////////////
    //void txPacket(void);
    //void rxPacket(void);
    byte txrxPacket(void);

    int motionBegin(void);
    void motionPlay(int wPage);
    /*
     * [ROBOTIS][ADD][START] 2013-04-09 support read and write on dxl bus
     * */
    void writeRaw(byte value);
    byte readRaw(void);
    byte available(void);
};


extern Dynamixel Dxl;


#endif /* DYNAMIXEL_H_ */
