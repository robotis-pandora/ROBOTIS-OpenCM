/*
 * dxl3.c
 *
 *  Created on: 2013. 10. 11.
 *      Author: in2storm
 */




/*
 * dxl.c
 *
 *  Created on: 2013. 4. 18.
 *      Author: in2storm
 */

#include "dxl3.h"
#include "delay.h"



void dxlInterruptUart3(byte data){
	gbpDXLDataBuffer3[gbDXLWritePointer3++ & 0xFF] = (uint8)USART3->regs->DR; //[ROBOTIS]Support to Dynamixel SDK.
}



uint32 Dummy3(uint32 tmp){
	return tmp;
}

void uDelay3(uint32 uTime) {
	uint32 cnt, max;
	static uint32 tmp = 0;

	for( max=0; max < uTime; max++)
	{
		for( cnt=0; cnt < 10 ; cnt++ )
		{
			tmp +=Dummy3(cnt);
		}
	}
	//tmpdly = tmp;
}
void nDelay3(uint32 nTime) { //100ns
	uint32 cnt, max;
	cnt=0;
	static uint32 tmp = 0;

	for( max=0; max < nTime; max++)
	{
		//for( cnt=0; cnt < 10 ; cnt++ )
		//{
			tmp +=Dummy3(cnt);
		//}
	}
	//tmpdly = tmp;
}

void clearBuffer256Uart3(void){
	gbDXLReadPointer3 = gbDXLWritePointer3 = 0;
}
byte checkNewArrive3(void){
	if(gbDXLReadPointer3 != gbDXLWritePointer3)
		return 1;
	else
		return 0;
}

void TxByteToDXL3(byte bTxData){
	USART3->regs->DR = (bTxData & (u16)0x01FF);
	while( (USART3->regs->SR & ((u16)0x0040)) == RESET );
}
byte RxByteFromDXL3(void){
	return gbpDXLDataBuffer3[gbDXLReadPointer3++ & 0xFF];
}


/**
 * @brief communicate with dynamixel bus on CM-9XX series. After transmitting packets to the bus, also it receives status feedback packets.
 * @param bID dynamixel ID
 * @param bInst Instruction number, you can find it from dynamixel.h
 * @param bTxParaLen length of packet
 *
 */

byte txrx_Packet3(byte bID, byte bInst, byte bTxParaLen){
	#define TRY_NUM 2//;;2

	byte bTxLen, bRxLenEx, bTryCount;

	gbBusUsed3 = 1;


	for(bTryCount = 0; bTryCount < TRY_NUM; bTryCount++)
	{
		gbDXLReadPointer3 = gbDXLWritePointer3; //BufferClear050728
		bTxLen = tx_Packet3(bID, bInst, bTxParaLen);

		if(bInst == INST_PING)
		{
			if(bID == BROADCAST_ID)
			{
				gbRxLength3 = bRxLenEx = 255;
			}
			else
			{
				gbRxLength3 = bRxLenEx = 6;
			}
		}
		else if(bInst == INST_READ)
		{
			gbRxLength3 = bRxLenEx = 6+gbpParameter3[1];
		}
		else if( bID == BROADCAST_ID )
		{
			gbRxLength3 = bRxLenEx = 0;
			break;
		}
		else
		{
			gbRxLength3 = bRxLenEx = 6;
		}


		if(bRxLenEx)//(gbpValidServo[bID] > 0x81 || bInst != INST_WRITE)) //ValidServo = 0x80|RETURN_LEVEL
		{
			gbRxLength3 = rx_Packet3(bRxLenEx);
			//TxDStringC("gbRxLength = ");TxD_Dec_U8C(gbRxLength);TxDStringC("\r\n");
			//TxDStringC("bRxLenEx = ");TxD_Dec_U8C(bRxLenEx);TxDStringC("\r\n");
			//      if(gbRxLength != bRxLenEx) //&& bRxLenEx != 255) before Ver 1.11e
			if((gbRxLength3 != bRxLenEx) && (bRxLenEx != 255)) // after Ver 1.11f
			{
				//TxDStringC(" Length mismatch!!\r\n");
				unsigned long ulCounter;
				word wTimeoutCount;
				ulCounter = 0;
				wTimeoutCount = 0;
				//TxDStringC("\r\n TEST POINT 0");
				while(ulCounter++ < RX_TIMEOUT_COUNT2)
				{
					if(gbDXLReadPointer3 != gbDXLWritePointer3) {
						gbDXLReadPointer3 = gbDXLWritePointer3; //BufferClear
						ulCounter = 0;
						if(wTimeoutCount++ > 100 )
						{
							//uDelay(0);// porting ydh added
							break; //porting ydh 100->245 //;;;;;; min max 뒤바뀜 found at Ver 1.11e
						}
						nDelay3(NANO_TIME_DELAY);// porting ydh added 20120210.
					}
					//uDelay(0);// porting ydh added
					nDelay3(NANO_TIME_DELAY);// porting ydh added

				}
				//TxDStringC("\r\n TEST POINT 1");
				gbDXLReadPointer3 = gbDXLWritePointer3; //BufferClear
			}
			else
			{
				//TxDStringC("\r\n TEST POINT 6");
				break;
			}
		}
	}

	//TxDStringC("\r\n TEST POINT 2");//TxDString("\r\n Err ID:0x");
	gbBusUsed3 = 0;

	if((gbRxLength3 != bRxLenEx) && (gbpTxBuffer3[2] != BROADCAST_ID))
	{
		//TxDByteC('3');//
		//TxDStringC("Rx Error\r\n");//TxDString("\r\n Err ID:0x");
#ifdef	PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
		//TxDString("\r\n Err ID:0x");
		//TxDHex8(bID);
		TxDStringC("\r\n ->[DXL]Err: ");
		PrintBuffer(gbpTxBuffer3,bTxLen3);
		TxDStringC("\r\n <-[DXL]Err: ");
		PrintBuffer(gbpRxBuffer3,gbRxLength3);
#endif

#ifdef	PRINT_OUT_TRACE_ERROR_PRINT_TO_USART2
		//TxDString("\r\n {[ERROR:");TxD16Hex(0x8100);TxDByte(':');TxD16Hex(bID);TxDByte(':');TxD8Hex(bInst);TxDByte(']');TxDByte('}');
		//TxDByte(bID);TxDByte(' ');
		//TxDByte(bInst);TxDByte(' ');
		//TxDByte(gbpParameter[0]);TxDByte(' ');
		//TxDByte(gbpParameter[1]);TxDByte(' ');
#endif
		return 0;
	}

	//TxDString("\r\n TEST POINT 4");//TxDString("\r\n Err ID:0x");
#ifdef PRINT_OUT_PACKET_TO_USART2
	TxDStringC("\r\n ->[TX Buffer]: ");
	PrintBuffer(gbpTxBuffer3,bTxLen);
	TxDStringC("\r\n <-[RX Buffer]: ");
	PrintBuffer(gbpRxBuffer3,gbRxLength);
#endif

	//gbLengthForPacketMaking =0;
	return 1;
}


//갯수에 맞게 받는 이유 : 통신에러가 나오면 Length가 틀릴 가능성이 무척 높기 때문
byte rx_Packet3(byte bRxLength){
	unsigned long ulCounter, ulTimeLimit;
	byte bCount, bLength, bChecksum;
	byte bTimeout;

	bTimeout = 0;
	if(bRxLength == 255) ulTimeLimit = RX_TIMEOUT_COUNT1;
	else ulTimeLimit = RX_TIMEOUT_COUNT2;
	for(bCount = 0; bCount < bRxLength; bCount++)
	{
		ulCounter = 0;
		while(gbDXLReadPointer3 == gbDXLWritePointer3)
		{
			nDelay3(NANO_TIME_DELAY); // porting ydh
			if(ulCounter++ > ulTimeLimit)
			{
				bTimeout = 1;
				//TxDStringC("Timeout\r\n");
				break;
			}
			uDelay3(0); //porting ydh added
		}
		if(bTimeout) break;
		gbpRxBuffer3[bCount] = gbpDXLDataBuffer3[gbDXLReadPointer3++];
		//TxDStringC("gbpRxBuffer = ");TxDHex8C(gbpRxBuffer[bCount]);TxDStringC("\r\n");
	}


	bLength = bCount;
	bChecksum = 0;

	if( gbpTxBuffer3[2] != BROADCAST_ID )
	{
		if(bTimeout && bRxLength != 255)
		{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
			TxDString("Rx Timeout");
			TxDByte(bLength);
#endif
			clearBuffer256();

			//return 0;
		}
		if(bLength > 3) //checking available length.
		{
			if(gbpRxBuffer3[0] != 0xff || gbpRxBuffer3[1] != 0xff )
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
				TxDStringC("Wrong Header");//[Wrong Header]
#endif
				clearBuffer256();
				return 0;
			}
			if(gbpRxBuffer3[2] != gbpTxBuffer3[2] )
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
				TxDStringC("[Error:TxID != RxID]");
#endif
				clearBuffer256Uart3();
				return 0;
			}
			if(gbpRxBuffer3[3] != bLength-4)
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
				TxDStringC("RxLength Error");
#endif
				clearBuffer256();
				return 0;
			}
			for(bCount = 2; bCount < bLength; bCount++) bChecksum += gbpRxBuffer3[bCount];
			if(bChecksum != 0xff)
			{
#ifdef PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
				TxDStringC("[RxChksum Error]");
#endif
				clearBuffer256Uart3();
				return 0;
			}
		}
	}

	return bLength;
}


byte tx_Packet3(byte bID, byte bInstruction, byte bParameterLength){
    byte bCount,bCheckSum,bPacketLength;

	gbpTxBuffer3[0] = 0xff;
    gbpTxBuffer3[1] = 0xff;
    gbpTxBuffer3[2] = bID;
    gbpTxBuffer3[3] = bParameterLength+2; //Length(Paramter,Instruction,Checksum)

    gbpTxBuffer3[4] = bInstruction;

    for(bCount = 0; bCount < bParameterLength; bCount++)
    {
        gbpTxBuffer3[bCount+5] = gbpParameter3[bCount];
    }
    bCheckSum = 0;
    bPacketLength = bParameterLength+4+2;

    for(bCount = 2; bCount < bPacketLength-1; bCount++) //except 0xff,checksum
    {
        bCheckSum += gbpTxBuffer3[bCount];
    }

    gbpTxBuffer3[bCount] = ~bCheckSum; //Writing Checksum with Bit Inversion

    DXL_TXD3; // this define is declared in dxl.h


    for(bCount = 0; bCount < bPacketLength; bCount++)
    {
        TxByteToDXL3(gbpTxBuffer3[bCount]);
    }

    DXL_RXD3;// this define is declared in dxl.h



    return(bPacketLength);
}
