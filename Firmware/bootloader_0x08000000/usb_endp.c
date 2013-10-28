/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "lib/USB_lib/inc/usb_lib.h"
#include "usb_desc.h"
#include "lib/USB_lib/inc/usb_mem.h"
#include "hw_config.h"
#include "USBInit.h"
#include "usb_istr.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define VCOMPORT_IN_FRAME_INTERVAL             5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];


extern volatile u8 gwpUSARTBuffer[];//extern volatile u16 gwpUSARTBuffer[];
extern volatile u16 gwUSARTWritePtr;
bool WriteFlash64(u8* RxBuffer, u16 RxCount );
extern vu32 gwAddressPointer,gwCalculatedCheckSum,gwRxTotalCount, gwReceivedCheckSumFromHost;
extern u8 gbFlashDownloadStart;
extern bool gbIsFlashLock;
extern __IO u32 bDeviceState;

#ifdef DEBUG_ENABLE_BY_USART2
extern void TxDString(char *str);
extern void TxDHex8(u16 bSentData);
extern void TxDHex16(u16 wSentData);
extern void TxDHex32(u32 lSentData);
#endif

extern void USB_TxDString(char *str);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    : When data send to Host PC by USB port, this function is called automatically
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback (void)
{

#ifdef STM32F10X_CL
      USB_SIL_Write(EP1_IN, &USART_Rx_Buffer[USB_Tx_ptr], USB_Tx_length);  
#else
     //UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
     // SetEPTxCount(ENDP1, USB_Tx_length);
     // SetEPTxValid(ENDP1);

#endif  

}

/*******************************************************************************
* Function Name  : EP3_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
volatile u16 USB_Rx_Cnt =0;
void EP3_OUT_Callback(void)
{

	/* Get the received data buffer and update the counter */
	USB_Rx_Cnt = USB_SIL_Read(EP3_OUT, USB_Rx_Buffer);
	/* USB data will be immediately processed, this allow next USB traffic beeing
	NAKed till the end of the USART Xfet */


//#ifdef DEBUG_ENABLE_BY_USART2
	//TxDString("[USB]USB_Rx_Cnt = ");	TxDHex16(USB_Rx_Cnt);	TxDString("\r\n");
//#endif

	if(gbFlashDownloadStart == TRUE){
		ClearTimeOutBuffer();
		WriteFlash64(USB_Rx_Buffer, USB_Rx_Cnt);
	}

	gwpUSARTBuffer[gwUSARTWritePtr++&USART_BUFFER_SIZE] = USB_Rx_Buffer[0];
	//TxDString("gwUSARTWritePtr = ");	TxDHex32(gwpUSARTBuffer[gwUSARTWritePtr-1]);	TxDString("\r\n");

  
#ifndef STM32F10X_CL
	/* Enable the receive of data on EP3 */
	SetEPRxValid(ENDP3);
#endif /* STM32F10X_CL */
}


bool WriteFlash64(u8* RxBuffer, u16 RxCount ){

	u16 lFlashCount;
	u8 lTheRestCount=0;
	volatile FLASH_Status FLASHStatus;

	if(gbIsFlashLock == TRUE) // If flash is lock, stop to write flash.
		return FALSE;

	lTheRestCount = 4 - (RxCount % 4);

	for(lFlashCount =0;lFlashCount < RxCount; lFlashCount += 4)
	{
#ifdef DEBUG_ENABLE_BY_USART2
		TxDByte(RxBuffer[lFlashCount]);
		TxDByte(RxBuffer[lFlashCount+1]);
		TxDByte(RxBuffer[lFlashCount+2]);
		TxDByte(RxBuffer[lFlashCount+3]);
#endif
		u32 data = (RxBuffer[lFlashCount+3]<<24)+(RxBuffer[lFlashCount+2]<<16)+(RxBuffer[lFlashCount+1]<<8)+RxBuffer[lFlashCount];
		FLASHStatus = FLASH_ProgramWord(gwAddressPointer, data);

		if( (*(vu32*)gwAddressPointer) != data )
		{
			USB_TxDString("\r\n Download Failed!");
			break;
		}

		if( gwAddressPointer > FLASH_END_ADDRESS )
		{
			USB_TxDString("\r\n Download Overflow!");
			break;
		}

		if( FLASHStatus != FLASH_COMPLETE ) // flash writing error
		{
			USB_TxDString("\r\n flash writing error!");
			break;
		}
		gwAddressPointer = gwAddressPointer + 4;

		gwCalculatedCheckSum += RxBuffer[lFlashCount+3]+RxBuffer[lFlashCount+2]+RxBuffer[lFlashCount+1]+RxBuffer[lFlashCount];
		//TxDString("loop CheckSum =");	TxDHex16(gwCalculatedCheckSum);	TxDString("\r\n");
		gwRxTotalCount+=4;

	}

	if(lTheRestCount != 4 ){
		//TxDString("lTheRestCount=");	TxDHex16(lTheRestCount);	TxDString("\r\n");
		gwRxTotalCount -= lTheRestCount;
		while(lTheRestCount){
			//TxDString("checksum deviation=");	TxDHex16(lFlashCount-lTheRestCount);	TxDString("\r\n");
			gwCalculatedCheckSum = gwCalculatedCheckSum - RxBuffer[(lFlashCount - lTheRestCount)];
			//TxDString("gwCalculatedCheckSum =");	TxDHex16(gwCalculatedCheckSum);	TxDString("\r\n");
			lTheRestCount--;
		}
	}

	return FALSE;
}

/*******************************************************************************
* Function Name  : SOF_Callback / INTR_SOFINTR_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
#ifdef STM32F10X_CL
void INTR_SOFINTR_Callback(void)
#else
void SOF_Callback(void)
#endif /* STM32F10X_CL */
{
  static u32 FrameCount = 0;
  
  if(bDeviceState == CONFIGURED)
  {
    if (FrameCount++ == VCOMPORT_IN_FRAME_INTERVAL)
    {
      /* Reset the frame counter */
      FrameCount = 0;
      
      /* Check the data to be sent through IN pipe -> it is always checked if sending data is exist*/
      //Handle_USBAsynchXfer();
    }
  }  
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

