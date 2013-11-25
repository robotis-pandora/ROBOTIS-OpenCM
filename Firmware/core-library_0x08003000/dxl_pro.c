/*
 * dxl_pro.c
 *
 *  Created on: 2013. 4. 25.
 *      Author: ROBOTIS,.LTD.
 */
#include <string.h>
#include "dxl_pro.h"


usart_dev *gDynamixelUsartDev = NULL;

uint32 Dummy(uint32 tmp){
	return tmp;
}

void uDelay(uint32 uTime) {
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
void nDelay(uint32 nTime) { //100ns
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
#ifdef CM9_DEBUG


void PrintBufferEx(byte *bpPrintBuffer, byte bLength)
{
	byte bCount;
	if(bLength == 0)
	{
		if(gbpTxBufferEx[2] == BROADCAST_ID)
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
}
#endif

uint32 dxl_get_baudrate(int baudnum)
{
    if(baudnum >= 2400)
        return baudnum;

    switch(baudnum)
    {
    case 0:
        return 2400;
    case 1:
        return 57600;
    case 2:
        return 115200;
    case 3:
        return 1000000;
    case 4:
        return 2000000;
    case 5:
        return 3000000;
    case 6:
        return 4000000;
    case 7:
        return 4500000;
    case 8:
        return 10500000;
    default:
        return 57600;
    }
}

unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)
{
	unsigned short i, j;
	unsigned short crc_table[256] = {0x0000,
	                                0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
	                                0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027,
	                                0x0022, 0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D,
	                                0x8077, 0x0072, 0x0050, 0x8055, 0x805F, 0x005A, 0x804B,
	                                0x004E, 0x0044, 0x8041, 0x80C3, 0x00C6, 0x00CC, 0x80C9,
	                                0x00D8, 0x80DD, 0x80D7, 0x00D2, 0x00F0, 0x80F5, 0x80FF,
	                                0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1, 0x00A0, 0x80A5,
	                                0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1, 0x8093,
	                                0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
	                                0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197,
	                                0x0192, 0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE,
	                                0x01A4, 0x81A1, 0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB,
	                                0x01FE, 0x01F4, 0x81F1, 0x81D3, 0x01D6, 0x01DC, 0x81D9,
	                                0x01C8, 0x81CD, 0x81C7, 0x01C2, 0x0140, 0x8145, 0x814F,
	                                0x014A, 0x815B, 0x015E, 0x0154, 0x8151, 0x8173, 0x0176,
	                                0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162, 0x8123,
	                                0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
	                                0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104,
	                                0x8101, 0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D,
	                                0x8317, 0x0312, 0x0330, 0x8335, 0x833F, 0x033A, 0x832B,
	                                0x032E, 0x0324, 0x8321, 0x0360, 0x8365, 0x836F, 0x036A,
	                                0x837B, 0x037E, 0x0374, 0x8371, 0x8353, 0x0356, 0x035C,
	                                0x8359, 0x0348, 0x834D, 0x8347, 0x0342, 0x03C0, 0x83C5,
	                                0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1, 0x83F3,
	                                0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
	                                0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7,
	                                0x03B2, 0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E,
	                                0x0384, 0x8381, 0x0280, 0x8285, 0x828F, 0x028A, 0x829B,
	                                0x029E, 0x0294, 0x8291, 0x82B3, 0x02B6, 0x02BC, 0x82B9,
	                                0x02A8, 0x82AD, 0x82A7, 0x02A2, 0x82E3, 0x02E6, 0x02EC,
	                                0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2, 0x02D0, 0x82D5,
	                                0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1, 0x8243,
	                                0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
	                                0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264,
	                                0x8261, 0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E,
	                                0x0234, 0x8231, 0x8213, 0x0216, 0x021C, 0x8219, 0x0208,
	                                0x820D, 0x8207, 0x0202 };

	for(j = 0; j < data_blk_size; j++)
	{
		i = ((unsigned short)(crc_accum >> 8) ^ *data_blk_ptr++) & 0xFF;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}

	return crc_accum;
}
void dxl_add_stuffing(unsigned char * packet)
{
    int i = 0, index = 0;
    int packet_length_in = DXL_MAKEWORD(packet[PKT_LENGTH_L], packet[PKT_LENGTH_H]);
    int packet_length_out = packet_length_in;
    unsigned char temp[MAXNUM_TXPACKET] = {0};

    memcpy(temp, packet, PKT_LENGTH_H + 1);    // FF FF FD XX ID LEN_L LEN_H
    index = PKT_INSTRUCTION;
    for( i = 0; i < packet_length_in - 2; i++) // except CRC
    {
        temp[index++] = packet[i+PKT_INSTRUCTION];
        if(packet[i+PKT_INSTRUCTION] == 0xFD && packet[i+PKT_INSTRUCTION-1] == 0xFF && packet[i+PKT_INSTRUCTION-2] == 0xFF)
        {   // FF FF FD
            temp[index++] = 0xFD;
            packet_length_out++;
        }
    }
    temp[index++] = packet[PKT_INSTRUCTION+packet_length_in-2];
    temp[index++] = packet[PKT_INSTRUCTION+packet_length_in-1];

   // if(packet_length_in != packet_length_out)
        //packet = (unsigned char*)realloc(packet, index * sizeof(unsigned char));

    memcpy(packet, temp, index);
    packet[PKT_LENGTH_L] = DXL_LOBYTE(packet_length_out);
    packet[PKT_LENGTH_H] = DXL_HIBYTE(packet_length_out);
}

void dxl_remove_stuffing(unsigned char * packet)
{
    int i = 0, index = 0;
    int packet_length_in = DXL_MAKEWORD(packet[PKT_LENGTH_L], packet[PKT_LENGTH_H]);
    int packet_length_out = packet_length_in;
    unsigned char temp[MAXNUM_TXPACKET] = {0};

    index = PKT_INSTRUCTION;
    for( i = 0; i < packet_length_in - 2; i++)  // except CRC
    {
        if(packet[i+PKT_INSTRUCTION] == 0xFD && packet[i+PKT_INSTRUCTION+1] == 0xFD && packet[i+PKT_INSTRUCTION-1] == 0xFF && packet[i+PKT_INSTRUCTION-2] == 0xFF)
        {   // FF FF FD FD
            packet_length_out--;
            i++;
        }
        packet[index++] = packet[i+PKT_INSTRUCTION];
    }
    packet[index++] = packet[PKT_INSTRUCTION+packet_length_in-2];
    packet[index++] = packet[PKT_INSTRUCTION+packet_length_in-1];

    packet[PKT_LENGTH_L] = DXL_LOBYTE(packet_length_out);
    packet[PKT_LENGTH_H] = DXL_HIBYTE(packet_length_out);
}

void dxlProInterrupt(byte data){
	gbpDXLDataBufferEx[gbDXLWritePointerEx++ & 0x3FF] = (uint8)gDynamixelUsartDev->regs->DR; //[ROBOTIS]Support to Dynamixel SDK.

}
void clearBuffer256Ex(void){
	gbDXLReadPointerEx = gbDXLWritePointerEx = 0;
}
byte checkNewArriveEx(void){
	if(gbDXLReadPointerEx != gbDXLWritePointerEx)
		return 1;
	else
		return 0;
}

void TxByteToDXLEx(byte bTxData){
	//USART_SendData(USART1,bTxData);
	//while( USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET );
	gDynamixelUsartDev->regs->DR = (bTxData & (u16)0x01FF);
	while( (gDynamixelUsartDev->regs->SR & ((u16)0x0040)) == RESET );
}
byte RxByteFromDXLEx(void){
/*	if(gbDXLWritePointerEx > 1023){ // prevent exceptions
		clearBuffer256Ex();
		return 0;
	}*/
	return gbpDXLDataBufferEx[gbDXLReadPointerEx++ & 0x3FF];
}


/**
 * @brief communicate with dynamixel bus on CM-9XX series. After transmitting packets to the bus, also it receives status feedback packets.
 * @param bID dynamixel ID
 * @param bInst Instruction number, you can find it from dynamixel.h
 * @param wTxParaLen length of packet
 *
 */

byte txrx_PacketEx(byte bID, byte bInst, word wTxParaLen){
	#define TRY_NUM 2//;;2

	word wTxLen, bRxLenEx, bTryCount;

	gbBusUsedEx = 1;


	for(bTryCount = 0; bTryCount < 1; bTryCount++)
	{
		gbDXLReadPointerEx = gbDXLWritePointerEx; //BufferClear050728

		/**************************************   Transfer packet  ***************************************************/
		wTxLen = tx_PacketEx(bID, bInst, wTxParaLen);

		if(bInst == INST_PING_EX)
		{
			if(bID == BROADCAST_ID)
			{
				gbRxLengthEx = bRxLenEx = 255;
			}
			else
			{
				gbRxLengthEx = bRxLenEx = 14;//6;
			}
		}
		else if(bInst == INST_READ_EX)
		{
			/*txpacket[PKT_PARAMETER+2]	= (unsigned char)DXL_LOBYTE(length);
				txpacket[PKT_PARAMETER+3]	= (unsigned char)DXL_HIBYTE(length);*/
			gbRxLengthEx = bRxLenEx = 11+DXL_MAKEWORD(gbpParameterEx[2], gbpParameterEx[3]);//6+gbpParameterEx[1];

		}
		else if( bID == BROADCAST_ID )
		{
			gbRxLengthEx = bRxLenEx = 0;
			break;
		}
		else
		{
			gbRxLengthEx = bRxLenEx = 11;//6;
		}


		if(bRxLenEx)//(gbpValidServo[bID] > 0x81 || bInst != INST_WRITE)) //ValidServo = 0x80|RETURN_LEVEL
		{
			/**************************************   Receive packet  ***************************************************/
			gbRxLengthEx = rx_PacketEx(bRxLenEx);


			//TxDStringC("gbRxLengthEx = ");TxD_Dec_U8C(gbRxLengthEx);TxDStringC("\r\n");
			//TxDStringC("bRxLenEx = ");TxD_Dec_U8C(bRxLenEx);TxDStringC("\r\n");
			//      if(gbRxLength != bRxLenEx) //&& bRxLenEx != 255) before Ver 1.11e
			if((gbRxLengthEx != bRxLenEx) && (bRxLenEx != 255)) // after Ver 1.11f
			{
				unsigned long ulCounter;
				word wTimeoutCount;
				ulCounter = 0;
				wTimeoutCount = 0;
				//TxDByteC('0');//TxDStringC("\r\n TEST POINT 0");//TxDString("\r\n Err ID:0x");
				while(ulCounter++ < RX_TIMEOUT_COUNT2)
				{
					if(gbDXLReadPointerEx != gbDXLWritePointerEx) {
						gbDXLReadPointerEx= gbDXLWritePointerEx; //BufferClear
						ulCounter = 0;
						if(wTimeoutCount++ > 100 )
						{
							//uDelay(0);// porting ydh added
							break; //porting ydh 100->245 //;;;;;; min max 뒤바뀜 found at Ver 1.11e
						}
						nDelay(NANO_TIME_DELAY);// porting ydh added 20120210.
					}
					//uDelay(0);// porting ydh added
					nDelay(NANO_TIME_DELAY);// porting ydh added

				}
				//TxDByteC('1');//TxDStringC("\r\n TEST POINT 1");//TxDString("\r\n Err ID:0x");
				gbDXLReadPointerEx = gbDXLWritePointerEx; //BufferClear
			}
			else
			{
				//TxDByteC('6');//TxDStringC("\r\n TEST POINT 6");//TxDString("\r\n Err ID:0x");
				break;
			}
		}
	}

	//TxDByteC('2');//TxDStringC("\r\n TEST POINT 2");//TxDString("\r\n Err ID:0x");
	gbBusUsedEx = 0;

	if((gbRxLengthEx != bRxLenEx) && (gbpTxBufferEx[4] != BROADCAST_ID))
	{
		//TxDByteC('3');//
		//TxDStringC("\r\n TEST POINT 3");//TxDString("\r\n Err ID:0x");
//#ifdef	PRINT_OUT_COMMUNICATION_ERROR_TO_USART2
		//TxDString("\r\n Err ID:0x");
		//TxDHex8(bID);
		//TxDStringC("\r\n ->[DXL]Err: ");
		//PrintBufferEx(gbpTxBufferEx,wTxLen);
		//TxDStringC("\r\n <-[DXL]Err: ");
		//PrintBufferEx(gbpRxBufferEx,gbRxLengthEx);
//#endif

		return 0;
	}

	//TxDString("\r\n TEST POINT 4");//TxDString("\r\n Err ID:0x");
#ifdef CM9_DEBUG
	/*TxDStringC("\r\n ->[TX Buffer]: ");
	PrintBufferEx(gbpTxBufferEx,wTxLen);
	TxDStringC("\r\n <-[RX Buffer]: ");
	PrintBufferEx(gbpRxBufferEx,gbRxLengthEx);*/
#endif

	//gbLengthForPacketMaking =0;
	return 1;
}


//갯수에 맞게 받는 이유 : 통신에러가 나오면 Length가 틀릴 가능성이 무척 높기 때문
byte rx_PacketEx(word wRxLength){
	unsigned long ulCounter, ulTimeLimit;
	byte bCount, bLength;
	word wCheckSum;
	byte bTimeout;

	bTimeout = 0;
	if(wRxLength == 255) ulTimeLimit = RX_TIMEOUT_COUNT1;
	else ulTimeLimit = RX_TIMEOUT_COUNT2;

	for(bCount = 0; bCount < wRxLength; bCount++)
	{
		ulCounter = 0;
		while(gbDXLReadPointerEx == gbDXLWritePointerEx)
		{
			nDelay(NANO_TIME_DELAY); // porting ydh
			if(ulCounter++ > ulTimeLimit)
			{
				bTimeout = 1;

				break;
			}
			uDelay(0); //porting ydh added
		}
		if(bTimeout) break;

		gbpRxBufferEx[bCount] = gbpDXLDataBufferEx[gbDXLReadPointerEx++];
		//TxDStringC("gbpRxBufferEx = ");TxDHex8C(gbpRxBufferEx[bCount]);TxDStringC("\r\n");
		if(gbDXLReadPointerEx > 255){
			clearBuffer256Ex();
			//TxDStringC("gbDXLReadPointerEx overflow!!\r\n");
		//	break;
		}
	}


	bLength = bCount;  // data length read from usart buffer
	//TxDStringC("read from bLength = ");TxDHex8C(bLength);TxDStringC("\r\n");
	wCheckSum = 0;
	//TxDStringC("gbpRxBufferEx[9] = ");TxDHex8C(gbpRxBufferEx[9]);TxDStringC("\r\n");

	if( gbpTxBufferEx[PKT_ID] != BROADCAST_ID )
	{
		if(bTimeout && wRxLength != 255)
		{
#ifdef CM9_DEBUG
			TxDStringC("Rx Timeout");
#endif
			clearBuffer256Ex();

			return 0;
		}
		if(bLength > 3) //checking available length.
		{
			if(gbpRxBufferEx[0] != 0xff || gbpRxBufferEx[1] != 0xff || gbpRxBufferEx[2] != 0xfd)
			{
#ifdef CM9_DEBUG
				TxDStringC("Wrong Header");//[Wrong Header]
#endif
				clearBuffer256Ex();
				return 0;
			}
			if(gbpRxBufferEx[PKT_ID] != gbpTxBufferEx[PKT_ID] )
			{
#ifdef CM9_DEBUG
				TxDStringC("[Error:TxID != RxID]");
#endif
				clearBuffer256Ex();
				return 0;
			}
			if(gbpRxBufferEx[5] != bLength-7)
			{
#ifdef CM9_DEBUG
				TxDStringC("RxLength Error");
#endif
				clearBuffer256Ex();
				return 0;
			}
			wCheckSum = DXL_MAKEWORD(gbpRxBufferEx[wRxLength-2], gbpRxBufferEx[wRxLength-1]);
			if(update_crc(0, gbpRxBufferEx, wRxLength-2) == wCheckSum){ // -2 : except CRC16

				return bLength;
			}
			else{
#ifdef CM9_DEBUG
				TxDStringC("CRC-16 Error\r\n");
#endif
				return 0;
			}

		}
	}

	return bLength;
}


byte tx_PacketEx(byte bID, byte bInstruction, word wParameterLength){

    word wCount, wCheckSum, wPacketLength;

    gbpTxBufferEx[0] = 0xff;
	gbpTxBufferEx[1] = 0xff;
	gbpTxBufferEx[2] = 0xfd;
	gbpTxBufferEx[3] = 0x00;

    gbpTxBufferEx[4] = bID;
    /*if(bInstruction == INST_SYNC_WRITE_EX){
    	gbpTxBufferEx[5] = DXL_LOBYTE(wParameterLength+7);// packet length = total parameter length(ID + data) + 7( start_addr + data_length + length + instruction )
    	gbpTxBufferEx[6] = DXL_HIBYTE(wParameterLength+7);
    }else*/
    {
		gbpTxBufferEx[5] = DXL_LOBYTE(wParameterLength+3);// packet length = total parameter length(address + data) + 3( length + instruction )
		gbpTxBufferEx[6] = DXL_HIBYTE(wParameterLength+3);
    }

    gbpTxBufferEx[7] = bInstruction;

    //TxDStringC("wParameterLength = ");TxDHex8C(wParameterLength);TxDStringC("\r\n");

    //TxDStringC("--------------\r\n");
   for(wCount = 0; wCount < wParameterLength; wCount++)
    {
        gbpTxBufferEx[wCount+8] = gbpParameterEx[wCount];  //push address and data to packet
       //TxDStringC("gbpParameterEx = ");TxDHex8C(gbpParameterEx[wCount]);TxDStringC("\r\n");
    }

    // Character stuffing
   // dxl_add_stuffing(gbpTxBufferEx);
    // Packet Header

    wCheckSum = 0;

   //wPacketLength = wParameterLength+8;// total packet length including packet header length
    wPacketLength = DXL_MAKEWORD(gbpTxBufferEx[5], gbpTxBufferEx[6])+5; //add packet header and ID length
   // TxDStringC("wPacketLength = ");TxDHex8C(wPacketLength);TxDStringC("\r\n");
    // Check MAX packet length
    if(wPacketLength > (MAXNUM_TXPACKET)){
        return 0;
    }
    // Add CRC16

    wCheckSum = update_crc(0, gbpTxBufferEx, wPacketLength);  // -2 : except CRC16

    gbpTxBufferEx[wPacketLength] = DXL_LOBYTE(wCheckSum);     // last - 1
    gbpTxBufferEx[wPacketLength+1] = DXL_HIBYTE(wCheckSum);     // last - 0


    wPacketLength += 2; //add crc length

	if(gDynamixelUsartDev == USART1){
		DXL_TXD
	}else{
		DXL_TXD3
	}

    for(wCount = 0; wCount < wPacketLength; wCount++)
    {
        TxByteToDXLEx(gbpTxBufferEx[wCount]);
    }

	if(gDynamixelUsartDev == USART1){
		DXL_RXD
	}else{
		DXL_RXD3
	}


    return(wPacketLength);
}


