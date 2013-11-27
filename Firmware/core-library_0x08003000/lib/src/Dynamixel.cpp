/*
 * Dynamixel.cpp
 *
 *  Created on: 2013. 11. 8.
 *      Author: in2storm
 */

#include "Dynamixel.h"
#include "nvic.h"
#include "Arduino-compatibles.h"
#include "dxl.h"

Dynamixel::Dynamixel(int dev_num) {
	// TODO Auto-generated constructor stub
	switch(dev_num){
	case 1:
		mDxlDevice = DXL_DEV1;
		mDxlUsart = USART1;
		//afio_remap(AFIO_REMAP_USART1);// not working here!!!

		mTxPort = PORT_DXL_TXD;
		mRxPort = PORT_DXL_RXD;

		mTxPin = PIN_DXL_TXD;
		mRxPin = PIN_DXL_RXD;
#ifndef BOARD_CM900
		mDirPort = PORT_TXRX_DIRECTION;
		mDirPin = PIN_TXRX_DIRECTION;
#endif
		
		break;
	case 2:
		/*
		 * USART2 is not used
		 *
		 */
		break;
	case 3:
		mDxlDevice = DXL_DEV3;
		//TxDStringC("\r\nStart new DXL codes!");
		mDxlUsart = USART3;
		mTxPort = PORT_DXL_TXD3;
		mRxPort = PORT_DXL_RXD3;
		mDirPort = PORT_TXRX_DIRECTION3;
		mTxPin = PIN_DXL_TXD3;
		mRxPin = PIN_DXL_RXD3;
		mDirPin = PIN_TXRX_DIRECTION3;
		//mDxlDevice = DXL_DEV3;
		break;
	default:
		break;

	}
}

Dynamixel::~Dynamixel() {
	// TODO Auto-generated destructor stub
}
void Dynamixel::begin(int baud){

	uint32 Baudrate = 0;
	if(mDxlUsart == USART1)
		afio_remap(AFIO_REMAP_USART1);

#ifdef BOARD_CM900  //Engineering version case
	 gpio_set_mode(PORT_ENABLE_TXD, PIN_ENABLE_TXD, GPIO_OUTPUT_PP);
	 gpio_set_mode(PORT_ENABLE_RXD, PIN_ENABLE_RXD, GPIO_OUTPUT_PP);
	 gpio_write_bit(PORT_ENABLE_TXD, PIN_ENABLE_TXD, 0 );// TX Disable
	 gpio_write_bit(PORT_ENABLE_RXD, PIN_ENABLE_RXD, 1 );// RX Enable
#else
	 gpio_set_mode(mDirPort, mDirPin, GPIO_OUTPUT_PP);
	 gpio_write_bit(mDirPort, mDirPin, 0 );// RX Enable
	 //gpio_set_mode(GPIOB, 5, GPIO_OUTPUT_PP);
	// gpio_write_bit(GPIOB, 5, 0 );// RX Enable
#endif
	// initialize GPIO D20(PB6), D21(PB7) as DXL TX, RX respectively
	gpio_set_mode(mTxPort, mTxPin, GPIO_AF_OUTPUT_PP);
	gpio_set_mode(mRxPort, mRxPin, GPIO_INPUT_FLOATING);
	//Initialize USART 1 device
	 usart_init(mDxlUsart);

	 //Calculate baudrate, refer to ROBOTIS support page.
	 Baudrate = 2000000 / (baud + 1);

	 if(mDxlUsart == USART1)
		 usart_set_baud_rate(mDxlUsart, STM32_PCLK2, Baudrate);
	 else
		 usart_set_baud_rate(mDxlUsart, STM32_PCLK1, Baudrate);
	nvic_irq_set_priority(mDxlUsart->irq_num, 0);//[ROBOTIS][ADD] 2013-04-10 set to priority 0
	usart_attach_interrupt(mDxlUsart, mDxlDevice->handlers);
	usart_enable(mDxlUsart);
	
	mDXLtxrxStatus = 0;
	mBusUsed = 0;// only 1 when tx/rx is operated
	//gbIsDynmixelUsed = 1;  //[ROBOTIS]2012-12-13 to notify end of using dynamixel SDK to uart.c
	this->clearBuffer();

	this->setLibStatusReturnLevel(2);
	this->setLibNumberTxRxAttempts(1);
	delay(100);


}
/*
 *  [ROBOTIS][ADD][START] 2013-04-09 support read and write on dxl bus
 * */

byte Dynamixel::readRaw(void){
	return mDxlDevice->data_buffer[(mDxlDevice->read_pointer)++ & 0xFF];;
}
void Dynamixel::writeRaw(uint8 value){

	dxlTxEnable();

	//TxByteToDXL(value);
	mDxlUsart->regs->DR = (value & (u16)0x01FF);
	while( (mDxlUsart->regs->SR & ((u16)0x0040)) == RESET );

	dxlTxDisable();
	//DXL_RXD
}

/*
 * @brief : if data coming from dxl bus, returns 1, or if not, returns 0.
 *
 */
byte Dynamixel::available(void){
	if(mDxlDevice->write_pointer != mDxlDevice->read_pointer)
		return 1;
	else
		return 0;
}
void Dynamixel::dxlTxEnable(void){
#ifdef BOARD_CM900  //Engineering version case
	 gpio_write_bit(PORT_ENABLE_TXD, PIN_ENABLE_TXD, 0 );// TX Disable
	 gpio_write_bit(PORT_ENABLE_RXD, PIN_ENABLE_RXD, 1 );// RX Enable
#else
	 gpio_write_bit(mDirPort, mDirPin, 1 );// RX Enable

#endif
}
void Dynamixel::dxlTxDisable(void){
#ifdef BOARD_CM900  //Engineering version case
	 gpio_write_bit(PORT_ENABLE_TXD, PIN_ENABLE_TXD, 0 );// TX Disable
	 gpio_write_bit(PORT_ENABLE_RXD, PIN_ENABLE_RXD, 1 );// RX Enable
#else
	 gpio_write_bit(mDirPort, mDirPin, 0 );// RX Enable
#endif
}
void Dynamixel::clearBuffer(void){
	mDxlDevice->read_pointer = 0;
	mDxlDevice->write_pointer = 0;

}

byte Dynamixel::txPacket(byte bID, byte bInstruction, byte bParameterLength){

    byte bCount,bCheckSum,bPacketLength;

	mTxBuffer[0] = 0xff;
	mTxBuffer[1] = 0xff;
	mTxBuffer[2] = bID;
	mTxBuffer[3] = bParameterLength+2; //Length(Paramter,Instruction,Checksum)

	mTxBuffer[4] = bInstruction;

    for(bCount = 0; bCount < bParameterLength; bCount++)
    {
    	mTxBuffer[bCount+5] = mParamBuffer[bCount];
    }
    bCheckSum = 0;
    bPacketLength = bParameterLength+4+2;

    for(bCount = 2; bCount < bPacketLength-1; bCount++) //except 0xff,checksum
    {
        bCheckSum += mTxBuffer[bCount];
    }

    mTxBuffer[bCount] = ~bCheckSum; //Writing Checksum with Bit Inversion

    dxlTxEnable(); // this define is declared in dxl.h


    for(bCount = 0; bCount < bPacketLength; bCount++)
    {
        writeRaw(mTxBuffer[bCount]);
    }

    dxlTxDisable();// this define is declared in dxl.h



    return(bPacketLength);
}
byte Dynamixel::rxPacket(byte bRxLength){
	unsigned long ulCounter, ulTimeLimit;
	byte bCount, bLength, bChecksum;
	byte bTimeout;

	bTimeout = 0;
	if(bRxLength == 255)
		ulTimeLimit = RX_TIMEOUT_COUNT1;
	else
		ulTimeLimit = RX_TIMEOUT_COUNT2;
	for(bCount = 0; bCount < bRxLength; bCount++)
	{
		ulCounter = 0;
		while(mDxlDevice->read_pointer == mDxlDevice->write_pointer)
		{
			nDelay(NANO_TIME_DELAY); //[ROBOTIS] porting ydh
			if(ulCounter++ > ulTimeLimit)
			{
				bTimeout = 1;
				//TxDStringC("Timeout\r\n");
				break;
			}
			uDelay(0); //[ROBOTIS] porting ydh added
		}
		if(bTimeout) break;
		mRxBuffer[bCount] = mDxlDevice->data_buffer[mDxlDevice->read_pointer++]; // get packet data from USART device
		//TxDStringC("mRxBuffer = ");TxDHex8C(mRxBuffer[bCount]);TxDStringC("\r\n");
	}

	bLength = bCount;
	bChecksum = 0;


	if( mTxBuffer[2] != BROADCAST_ID )
	{
		if(bTimeout && bRxLength != 255)
		{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
			TxDStringC("Rx Timeout");
			TxDByteC(bLength);
#endif
			mDXLtxrxStatus |= (1<<COMM_RXTIMEOUT);
			clearBuffer();
			//TxDStringC("Rx Timeout");
			return 0;
		}
		if(bLength > 3) //checking available length.
		{
			if(mRxBuffer[0] != 0xff || mRxBuffer[1] != 0xff )
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
				TxDStringC("Wrong Header");//[Wrong Header]
#endif
				mDXLtxrxStatus |= (1<<COMM_RXCORRUPT);//RXHEADER);
				clearBuffer();
				return 0;
			}
			if(mRxBuffer[2] != mTxBuffer[2] )
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
				TxDStringC("[Error:TxID != RxID]");
#endif
				mDXLtxrxStatus |= (1<<COMM_RXCORRUPT);//RXID);
				clearBuffer();
				return 0;
			}
			if(mRxBuffer[3] != bLength-4)
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
				TxDStringC("RxLength Error");
#endif
				mDXLtxrxStatus |= (1<<COMM_RXCORRUPT);//RXLENGTH);
				clearBuffer();
				return 0;
			}
			for(bCount = 2; bCount < bLength; bCount++){
				bChecksum += mRxBuffer[bCount]; //Calculate checksum of received data for compare
			}
			if(bChecksum != 0xff)
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
				TxDStringC("[RxChksum Error]");
#endif
				mDXLtxrxStatus |= (1<<COMM_RXCORRUPT);//RXCHECKSUM);
				clearBuffer();
				return 0;
			}
		}
	}

	return bLength;
}
byte Dynamixel::txRxPacket(byte bID, byte bInst, byte bTxParaLen){

//#define TRY_NUM 2//;;2

	mDXLtxrxStatus = 0;

	byte bTxLen, bRxLenEx, bTryCount;

	mBusUsed = 1;
	mRxLength = bRxLenEx = 0;

	for(bTryCount = 0; bTryCount < gbDXLNumberTxRxAttempts; bTryCount++)//for(bTryCount = 0; bTryCount < TRY_NUM; bTryCount++)
	{
		//gbDXLReadPointer = gbDXLWritePointer;
		mDxlDevice->read_pointer = mDxlDevice->write_pointer;//[ROBOTIS]BufferClear050728
		bTxLen = this->txPacket(bID, bInst, bTxParaLen);

		if (bTxLen == (bTxParaLen+4+2)){
			mDXLtxrxStatus = (1<<COMM_TXSUCCESS);
		}
//		else{
//			return 0;
//		}

		if(bInst == INST_PING){
			if(bID == BROADCAST_ID){
				mRxLength = bRxLenEx = 255; //
			}
			else{
				mRxLength = bRxLenEx = 6; // basic response packet length
			}
		}
		else if(bInst == INST_READ){
			//mRxLength = bRxLenEx = 6+mParamBuffer[1]; // basic response packet length + requested data length in read instruction
			if (gbDXLStatusReturnLevel>0)
				mRxLength = bRxLenEx = 6+mParamBuffer[1];
			else
				mRxLength = bRxLenEx = 0;

		}
		else if( bID == BROADCAST_ID ){
			mRxLength = bRxLenEx = 0; // no response packet in case broadcast id
			break;
		}
		else{
			//mRxLength = bRxLenEx = 6; //basic response packet length
			if (gbDXLStatusReturnLevel>1)
				mRxLength = bRxLenEx = 6+mParamBuffer[1];
			else
				mRxLength = bRxLenEx = 0;
		}


		if(bRxLenEx){//(gbpValidServo[bID] > 0x81 || bInst != INST_WRITE)) //ValidServo = 0x80|RETURN_LEVEL
			mRxLength = this->rxPacket(bRxLenEx);
			//TxDStringC("gbRxLength = ");TxDHex8C(mRxLength);TxDStringC("\r\n");
			//TxDStringC("bRxLenEx = ");TxDHex8C(bRxLenEx);TxDStringC("\r\n");
			//      if(gbRxLength != bRxLenEx) //&& bRxLenEx != 255) before Ver 1.11e
			if((mRxLength != bRxLenEx) && (bRxLenEx != 255)) // after Ver 1.11f
			{
				//TxDStringC(" Length mismatch!!\r\n");
				unsigned long ulCounter;
				word wTimeoutCount;
				ulCounter = 0;
				wTimeoutCount = 0;
				//TxDStringC("\r\n TEST POINT 0");
				while(ulCounter++ < RX_TIMEOUT_COUNT2)
				{
					//if(gbDXLReadPointer != gbDXLWritePointer)
					if(this->available())  //data is available in dxl bus
					{
						mDxlDevice->read_pointer = mDxlDevice->write_pointer;// gbDXLWritePointer; //BufferClear
						ulCounter = 0;
						if(wTimeoutCount++ > 100 )
						{
							//uDelay(0);// porting ydh added
							break; //porting ydh 100->245 //;;;;;; min max µÚ¹Ù²ñ found at Ver 1.11e
						}
						nDelay(NANO_TIME_DELAY);// porting ydh added 20120210.
					}
					//uDelay(0);// porting ydh added
					nDelay(NANO_TIME_DELAY);// porting ydh added

				}
				//TxDStringC("\r\n TEST POINT 111");
				mDxlDevice->read_pointer = mDxlDevice->write_pointer; //BufferClear
			}
			else{
				//TxDStringC("\r\n TEST POINT 6");
				break;
			}
		}//bRxLenEx is exist
	}

	//TxDStringC("\r\n TEST POINT 2");//TxDString("\r\n Err ID:0x");
	mBusUsed = 0;

	if((mRxLength != bRxLenEx) && (mTxBuffer[2] != BROADCAST_ID))
	{
		//TxDByteC('3');//
		//TxDStringC("Rx Error\r\n");//TxDString("\r\n Err ID:0x");
#ifdef	PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
		//TxDString("\r\n Err ID:0x");
		//TxDHex8(bID);
		TxDStringC("\r\n ->[DXL]Err: ");
		PrintBuffer(mTxBuffer,bTxLen);
		TxDStringC("\r\n <-[DXL]Err: ");
		PrintBuffer(mRxBuffer,mRxLength);
#endif

#ifdef	PRINT_OUT_TRACE_ERROR_PRINT_TO_USART2
		//TxDString("\r\n {[ERROR:");TxD16Hex(0x8100);TxDByte(':');TxD16Hex(bID);TxDByte(':');TxD8Hex(bInst);TxDByte(']');TxDByte('}');
		//TxDByte(bID);TxDByte(' ');
		//TxDByte(bInst);TxDByte(' ');
		//TxDByte(gbpParameter[0]);TxDByte(' ');
		//TxDByte(gbpParameter[1]);TxDByte(' ');
#endif
		return 0;
	}else if((mRxLength == 0) && (mTxBuffer[4] == INST_PING)){  //[ROBOTIS] 2013-11-22 correct response for ping instruction
		return 0;
	}
	//TxDString("\r\n TEST POINT 4");//TxDString("\r\n Err ID:0x");
#ifdef PRINT_OUT_PACKET_TO_USART2
	TxDStringC("\r\n ->[TX Buffer]: ");
	printBuffer(mTxBuffer,bTxLen);
	TxDStringC("\r\n <-[RX Buffer]: ");
	printBuffer(mRxBuffer,mRxLength);
#endif
	mDXLtxrxStatus = (1<<COMM_RXSUCCESS);

	//gbLengthForPacketMaking =0;
	return 1;
}

byte Dynamixel::writeByte(byte bID, byte bAddress, byte bData){

	mParamBuffer[0] = bAddress;
	mParamBuffer[1] = bData;
	return this->txRxPacket(bID, INST_WRITE, 2);

}


byte Dynamixel::readByte(byte bID, byte bAddress){
	mParamBuffer[0] = bAddress;
	mParamBuffer[1] = 1;
	if( this->txRxPacket(bID, INST_READ, 2)){
		//mCommStatus = 1;
		return(mRxBuffer[5]);
	}
	else{
		//mCommStatus = 0;
		return 0xff;
	}
}

word Dynamixel::readWord(byte bID, byte bAddress){
	mParamBuffer[0] = bAddress;
	mParamBuffer[1] = 2;
	if(this->txRxPacket(bID, INST_READ, 2))
	{
		//mCommStatus = 1;
		return( (((word)mRxBuffer[6])<<8)+ mRxBuffer[5] );
	}
	else{
		//mCommStatus = 0;
		return 0xffff;
	}
}



byte Dynamixel::writeWord(byte bID, byte bAddress, word wData){
	mParamBuffer[0] = bAddress;
    mParamBuffer[1] = (byte)(wData&0xff);
    mParamBuffer[2] = (byte)((wData>>8)&0xff);

    return this->txRxPacket(bID, INST_WRITE, 3);
}
/*
 * @brief Sets the target position and speed of the specified servo
 * @author Made by Martin S. Mason(Professor @Mt. San Antonio College)
 * @change 2013-04-17 changed by ROBOTIS,.LTD.
 * */
byte Dynamixel::setPosition(byte ServoID, int Position, int Speed){
	mParamBuffer[0] = (unsigned char)30;
	mParamBuffer[1] = (unsigned char)DXL_LOBYTE(Position);
	mParamBuffer[2] = (unsigned char)DXL_HIBYTE(Position);
	mParamBuffer[3] = (unsigned char)DXL_LOBYTE(Speed);
	mParamBuffer[4] = (unsigned char)DXL_HIBYTE(Speed);
    return(this->txRxPacket(ServoID, INST_WRITE, 5));
}

/*
 * @Depreciated, use DXL_LOBYTE(w) instead of getLowByte() method
 * */
byte Dynamixel::getLowByte( word wData ){

	return (byte)(wData & 0xff);
}
/*
 * @Depreciated, use DXL_HIBYTE(w) instead of getHighByte() method
 * */
byte Dynamixel::getHighByte( word wData ){

	return ((wData & 0xff00) >> 8);
}
/*
 * @Depreciated, use DXL_MAKEWORD(w) instead of makeWord() method
 * */
word  Dynamixel::makeWord( byte lowbyte, byte highbyte ){
	word wData;

	wData = ((highbyte & 0x00ff) << 8);
	return (wData + lowbyte);
}

byte  Dynamixel::ping( byte  bID ){

	if(this->txRxPacket(bID, INST_PING, 0)){
		return mRxBuffer[2]; //return id if exist
	}else{
		return 0xff;  //no dxl in bus.
	}

}
byte  Dynamixel::reset( byte  bID ){

	return(this->txRxPacket(bID, INST_RESET, 0));
}
/*
 * Use getTxRxStatus() instead of getResult()
 * */
byte  Dynamixel::getResult(void){
	//	return mCommStatus;
	return this->getTxRxStatus();
}
byte Dynamixel::getTxRxStatus(void) // made by NaN (Robotsource.org)
{
	return mDXLtxrxStatus;
}
byte Dynamixel::setLibStatusReturnLevel(byte num)
{
	gbDXLStatusReturnLevel = num;
	return gbDXLStatusReturnLevel;
}

byte Dynamixel::setLibNumberTxRxAttempts(byte num)
{
	gbDXLNumberTxRxAttempts = num;
	return gbDXLNumberTxRxAttempts;
}
void Dynamixel::printBuffer(byte *bpPrintBuffer, byte bLength)
{
#ifdef	PRINT_OUT_TRACE_ERROR_PRINT_TO_USART2
	byte bCount;
	if(bLength == 0)
	{
		if(mTxBuffer[2] == BROADCAST_ID)
		{
			TxDStringC("\r\n No Data[at Broadcast ID 0xFE]");
		}
		else
		{
			TxDStringC("\r\n No Data(Check ID, Operating Mode, Baud rate)");//TxDString("\r\n No Data(Check ID, Operating Mode, Baud rate)");
		}
	}
	for(bCount = 0; bCount < bLength; bCount++)
	{
		TxDHex8C(bpPrintBuffer[bCount]);
		TxDByteC(' ');
	}
	TxDStringC(" LEN:");//("(LEN:")
	TxDHex8C(bLength);
	TxDStringC("\r\n");
#endif
}

uint32 Dynamixel::Dummy(uint32 tmp){
	return tmp;
}
void Dynamixel::uDelay(uint32 uTime){
	uint32 cnt, max;
		static uint32 tmp = 0;

		for( max=0; max < uTime; max++)
		{
			for( cnt=0; cnt < 10 ; cnt++ )
			{
				tmp +=Dummy(cnt);
			}
		}
		//tmpdly = tmp;
}
void Dynamixel::nDelay(uint32 nTime){
	uint32 cnt, max;
		cnt=0;
		static uint32 tmp = 0;

		for( max=0; max < nTime; max++)
		{
			//for( cnt=0; cnt < 10 ; cnt++ )
			//{
				tmp +=Dummy(cnt);
			//}
		}
		//tmpdly = tmp;
}


/*
 * @brief initialize parameter and get ID, instruction for making packet
 * @start_addr : start address for sync write
 * @num_of_data : DATA1, DATA2.. -> number of data
 * @Param : packet parameter array for controlling all dxls in bus { ID1, DATA1, DATA2,..., ID2, DATA1, DATA2,..., ID3, DATA1, DATA2,... }
 * @array_length : actual length of parameter array
 */
byte Dynamixel::syncWrite(byte start_addr, byte num_of_data, byte *param, int array_length){
	int i;
	mbLengthForPacketMaking = 0;
	mbIDForPacketMaking = BROADCAST_ID;
	mbInstructionForPacketMaking = INST_SYNC_WRITE;
	mCommStatus = 0;
	mParamBuffer[mbLengthForPacketMaking++] = start_addr;
	mParamBuffer[mbLengthForPacketMaking++] = num_of_data; // actual number of data as byte
	for(i=0; i < array_length; i++){
		mParamBuffer[mbLengthForPacketMaking++] = param[i];
	}
	mCommStatus = this->txRxPacket(mbIDForPacketMaking, mbInstructionForPacketMaking, mbLengthForPacketMaking);
	return mCommStatus;
}
byte Dynamixel::syncWrite(byte start_addr, byte num_of_data, int *param, int array_length){
	int i,j,k,num;
	k=0;
	num = array_length/(1+num_of_data); //ID+DATA1+DATA2..
	mbLengthForPacketMaking = 0;
	mbIDForPacketMaking = BROADCAST_ID;
	mbInstructionForPacketMaking = INST_SYNC_WRITE;
	mCommStatus = 0;
	mParamBuffer[mbLengthForPacketMaking++] = start_addr;
	mParamBuffer[mbLengthForPacketMaking++] = num_of_data*2;
	for(i=mbLengthForPacketMaking; i < num*(1+num_of_data*2); i+=(1+num_of_data*2)){
		mParamBuffer[i] = param[k++]; //ID
		for(j=0; j < (num_of_data*2); j+=2){
			mParamBuffer[i+j+1] = (unsigned char)DXL_LOBYTE(param[k]); //low byte
			mParamBuffer[i+j+2] = (unsigned char)DXL_HIBYTE(param[k]);; //high byte
			k++;
		}
	}
	mbLengthForPacketMaking= i;
	mCommStatus = this->txRxPacket(mbIDForPacketMaking, mbInstructionForPacketMaking, mbLengthForPacketMaking);
	return mCommStatus;
}

/*
 * @brief initialize parameter and get ID, instruction for making packet
 * */
void Dynamixel::initPacket(byte bID, byte bInst){
	mbLengthForPacketMaking = 0;
	mbIDForPacketMaking = bID;
	mbInstructionForPacketMaking = bInst;
	mCommStatus = 0;
}
/*
 * @brief just push parameters, individual ID or moving data, individual data length
 * */
void Dynamixel::pushByte(byte value){
	//packet length is not above the maximum 143 bytes because size of buffer receiver has only 143 bytes capacity.
	//please refer to ROBOTIS e-manual (support.robotis.com)
	if(mbLengthForPacketMaking > 140)//prevent violation of memory access
		return;
	mParamBuffer[mbLengthForPacketMaking++] = value;
}
void Dynamixel::pushParam(byte value){
	if(mbLengthForPacketMaking > 140)//prevent violation of memory access
			return;
	mParamBuffer[mbLengthForPacketMaking++] = value;
}
void Dynamixel::pushParam(int value){

	if(mbLengthForPacketMaking > 140)//prevent violation of memory access
			return;
	mParamBuffer[mbLengthForPacketMaking++] = (unsigned char)DXL_LOBYTE(value);
	mParamBuffer[mbLengthForPacketMaking++] = (unsigned char)DXL_HIBYTE(value);
}
/*
 * @brief transfers packets to dynamixel bus
 * */
byte Dynamixel::flushPacket(void){

	//TxDString("\r\n");
	//TxD_Dec_U8(gbLengthForPacketMaking);
	mCommStatus = this->txRxPacket(mbIDForPacketMaking, mbInstructionForPacketMaking, mbLengthForPacketMaking);
	return mCommStatus;
}
/*
 * @brief return current the total packet length
 * */
byte Dynamixel::getPacketLength(void){
	return mbLengthForPacketMaking;
}



void Dynamixel::setTxPacketId(byte id){
	mbIDForPacketMaking = id;

}
void Dynamixel::setTxPacketInstruction(byte instruction){
	mbInstructionForPacketMaking = instruction;

}
void Dynamixel::setTxPacketParameter( byte index, byte value ){
	mParamBuffer[index] = value;

}
void Dynamixel::setTxPacketLength( byte length ){
	mbLengthForPacketMaking = length;

}
byte Dynamixel::txrxPacket(void){
	mCommStatus = this->txRxPacket(mbIDForPacketMaking, mbInstructionForPacketMaking, mbLengthForPacketMaking);
	return mCommStatus;
}

int Dynamixel::getRxPacketParameter( int index ){
	//return dxl_get_rxpacket_parameter( index );
	return mRxBuffer[5 + index];
}
int Dynamixel::getRxPacketLength(void){
	//return dxl_get_rxpacket_length();
	return mRxBuffer[3]; //length index is 3 in status packet
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
int Dynamixel::getRxPacketError( byte errbit ){
	//return dxl_get_rxpacket_error( errbit );
	if( mRxBuffer[4] & errbit ){ //error bit index is 4 in status packet
		return 1;
	}
	return 0;

}
