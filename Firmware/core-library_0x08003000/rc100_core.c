// Zigbee SDK platform independent source

#include "rc100_core.h"
//#include "rc100_hal.h"
#include "usart.h"
//#include "stm32.h" //2013-06-17
#include "gpio.h"

unsigned char gbRcvPacket[6];
unsigned char gbRcvPacketNum;
unsigned short gwRcvData;
unsigned char gbRcvFlag;



void TxDByteUart2(byte bTxdData)
{
	/*USART_SendData(USART1,bTxdData);
	while( USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET );
	*/
	USART2->regs->DR = (bTxdData & (u16)0x01FF);
	while( (USART2->regs->SR & ((u16)0x0040)) == RESET );


}

byte RxDByteUart2(void)
{
	byte bTemp;

	bTemp = gbpPacketDataBuffer[gbPacketReadPointer++];

	gbPacketReadPointer = gbPacketReadPointer & 0x1F; //added 2012-11-23
	return bTemp;
}


int rc100_hal_tx( unsigned char *pPacket, int numPacket )
{
	// Transmiting date
	// *pPacket: data array pointer
	// numPacket: number of data array
	// Return: number of data transmitted. -1 is error.
	unsigned char i;
	for(i=0 ; i<numPacket; i++  ){
		TxDByteUart2(pPacket[i]);
	}

	return numPacket;
}

byte CheckNewArrive(void)
{
	if(gbPacketReadPointer != gbPacketWritePointer)
		return 1;
	else
		return 0;
}

int rc100_hal_rx( unsigned char *pPacket, int numPacket )
{
	// Recieving date
	// *pPacket: data array pointer
	// numPacket: number of data array
	// Return: number of data recieved. -1 is error.
	unsigned char i;
	for( i=0 ; i<numPacket ; i++ )
	{
		if (CheckNewArrive())
			pPacket[i] =  RxDByteUart2();

		else
			return i;
	}

	return numPacket;
}

void rc100Interrupt(byte data){
	gbpPacketDataBuffer[gbPacketWritePointer++] = data;
	gbPacketWritePointer = gbPacketWritePointer & 0x1F;

}

int rc100Initialize(uint32 baudrate )
{
/*
 *  Opening device
 *  baudrate: Real baudrate (ex> 115200, 57600, 38400...)
 *  Return: 0(Failed), 1(Succeed)
 *
 * */
	gpio_set_mode(GPIOA, 2, GPIO_AF_OUTPUT_PP);
	gpio_set_mode(GPIOA, 3, GPIO_INPUT_FLOATING);
	usart_init(USART2);
	//TxDStringC("USART clock = ");TxDHex32C(STM32_PCLK2);TxDStringC("\r\n");
	usart_set_baud_rate(USART2, STM32_PCLK1, baudrate);
	usart_attach_interrupt(USART2,rc100Interrupt);
	usart_enable(USART2);

	gbRcvFlag = 0;
	gwRcvData = 0;
	gbRcvPacketNum = 0;
	/*Clear rx tx uart2 buffer */
	gbPacketWritePointer =0;
	gbPacketReadPointer=0;

	return 1;
}

void rc100Terminate(void)
{

// Closing device
	usart_detach_interrupt(USART2);
	usart_disable(USART2);

}

int rc100TxData(int data)
{
	unsigned char SndPacket[6];
	unsigned short word = (unsigned short)data;
	unsigned char lowbyte = (unsigned char)(word & 0xff);
	unsigned char highbyte = (unsigned char)((word >> 8) & 0xff);

	SndPacket[0] = 0xff;
	SndPacket[1] = 0x55;
	SndPacket[2] = lowbyte;
	SndPacket[3] = ~lowbyte;
	SndPacket[4] = highbyte;
	SndPacket[5] = ~highbyte;

	if( rc100_hal_tx( SndPacket, 6 ) != 6 )
		return 0;

	return 1;
}
/*
extern volatile byte                   gbPacketWritePointer; // PC, Wireless
extern volatile byte                   gbPacketReadPointer ;
*/

int rc100RxCheck(void)
{
	int RcvNum;
	unsigned char checksum;
	int i, j;

	if(gbRcvFlag == 1)
		return 1;

	// Fill packet buffer
	if(gbRcvPacketNum < 6)
	{
		RcvNum = rc100_hal_rx( &gbRcvPacket[gbRcvPacketNum], (6 - gbRcvPacketNum) );
		if( RcvNum != -1 )
			gbRcvPacketNum += RcvNum;
	}

	// Find header
	if(gbRcvPacketNum >= 2)
	{
		for( i=0; i<gbRcvPacketNum; i++ )
		{
			if(gbRcvPacket[i] == 0xff)
			{
				if(i <= (gbRcvPacketNum - 2))
				{
					if(gbRcvPacket[i+1] == 0x55)
						break;
				}
			}
		}

		if(i > 0)
		{
			if(i == gbRcvPacketNum)
			{
				// Can not find header
				if(gbRcvPacket[i - 1] == 0xff)
					i--;
			}

			// Remove data before header
			for( j=i; j<gbRcvPacketNum; j++)
			{
				gbRcvPacket[j - i] = gbRcvPacket[j];
			}
			gbRcvPacketNum -= i;
		}
	}

	// Verify packet
	if(gbRcvPacketNum == 6)
	{
		if(gbRcvPacket[0] == 0xff && gbRcvPacket[1] == 0x55)
		{
			checksum = ~gbRcvPacket[3];
			if(gbRcvPacket[2] == checksum)
			{
				checksum = ~gbRcvPacket[5];
				if(gbRcvPacket[4] == checksum)
				{
					gwRcvData = (unsigned short)((gbRcvPacket[4] << 8) & 0xff00);
					gwRcvData += gbRcvPacket[2];
					gbRcvFlag = 1;
				}
			}
		}

		gbRcvPacket[0] = 0x00;
		gbRcvPacketNum = 0;
	}


	return gbRcvFlag;
}

int rc100RxData(void)
{
	gbRcvFlag = 0;
	return (int)gwRcvData;
}
