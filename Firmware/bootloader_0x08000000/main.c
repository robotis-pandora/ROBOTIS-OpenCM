/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : Main program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* 
 * @File: main.c
 * @Brief : main function for the bootloader of cm-9 series board.
 * changed by ROBOTIS,.LTD.
 */

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_lib.h"
#include "USBInit.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

typedef  void (*pFunction)(void);

/*
 * CM-900 Compile Option
 * 2012-08-29 ROBOTIS,.LTD. sm6787@robotis.com
 * */
//#define DEBUG_ENABLE_BY_USART2
//#define POWER_SOURCE_DETECT
//#define USE_USB_POWER_MANAGEMENT
//#define USE_EEPROM_EMULATOR


#define COMMAND_LENGTH 16
#define COMMAND_BUFFER_SIZE 80
#define PARA_NUM 10

#define P_OPERATING_MODE      19

/* Private macro -------------------------------------------------------------*/

#define USART_BUFFER_CLEAR	{gwUSARTReadPtr=0; gwUSARTWritePtr=0;}
#define RXD_BUFFER_READY	(gwUSARTReadPtr != gwUSARTWritePtr)

#define __disable_interrupt() __SETPRIMASK()
#define __enable_interrupt()  __RESETPRIMASK()

/* Private variables ---------------------------------------------------------*/

ErrorStatus HSEStartUpStatus;
USART_InitTypeDef USART_InitStructure;

volatile u8 gwpUSARTBuffer[USART_BUFFER_SIZE+1];//volatile u16 gwpUSARTBuffer[USART_BUFFER_SIZE+1];
volatile u16 gwUSARTReadPtr, gwUSARTWritePtr;
//char gbpParameter[30];
char gbpRxBuffer[81];

vu32 TimingDelay;

u32 gwWriteProtectedPages=0;	

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void Timer_Configuration(void);
#ifdef POWER_SOURCE_DETECT
void ADC_Configuration(void);
#endif

//void USART_Configuration(u32 nBaud);
void USART_Configuration(u8 PORT, u32 baudrate);

void TxDByte(u8 dat);
void TxD_Dec_U8(u8 bByte);  // 2012-05-15 jason added from CM-530 Bootloader 1.01
void TxDString(char *str);
void TxDHex8(u16 bSentData);
void TxDHex16(u16 wSentData);
void TxDHex32(u32 lSentData);

void SerialMonitor(void);
u16 StringProcess(char *bpCommand, u32 *wpPara, char *bpString);
u16 GetCommandFromHost(char *bpStringPointer);
u16 StringCompare(char *bpA, char *bpB,int limit);
u16 StringCopy(char *bpDst, char *bpSrc);
void Delay(u32 nTime);
//void PrintProtectStatus();
#ifdef USE_EEPROM_EMULATOR
u16 EEPROM_Read( u32 Offset );
void EEPROM_Write( u32 Offset, u16 Data );
void EEPROM_Clear( void );
#endif

void USB_TxDByte(u8 dat);
void USB_TxDString(char *str);
extern void USB_TxDHex8(u16 bSentData);
void USB_TxDHex16(u16 wSentData);
void USB_TxDHex32(u32 lSentData);
void USB_TxD_Dec_U8(u8 bByte);
u8 RxDBufferFromHost(void);

u8 CheckTimeOut(void);
void Interrupt1ms(void);
vu32  gu32TimingCounter1ms = 0;
//vu32  gw1msCounter=0;
void ClearTimeOutBuffer(void);

u32 dxl_get_baudrate(int baudnum, u8 option);
volatile u8  gbDXLWritePointer;
volatile u8  gbDXLReadPointer;
volatile u8  gbpDXLDataBuffer[256];
void ClearBuffer256(void);


vu32 gwAddressPointer=0, gwRxTotalCount=0, gwReceivedCheckSumFromHost=0;
vu32 gwCalculatedCheckSum=0;
vu32 gwEndAddressPointer=0;



/*
 * If flash download is start, gbFlashDownloadStart variable is set to TRUE
 * and the download process is occurred in USB EP3_OUT_Callback function.
 * Otherwise, it is set to FALSE
 *
 * */
bool gbFlashDownloadStart=FALSE;
/*
 * If flash is locked, gbIsFlashLock variable is TRUE
 * If unlocked, it is FALSE;
 *
 * */
bool gbIsFlashLock=TRUE;

extern __IO u32 bDeviceState;

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	/* System Clocks Configuration */
	RCC_Configuration();

	/* Configure the GPIO ports */
	GPIO_Configuration();

	/* NVIC configuration */
	NVIC_Configuration();

	Timer_Configuration();

	/* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
	SysTick_SetReload(9000);

	/* Enable SysTick interrupt */
	SysTick_ITConfig(ENABLE);

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	IWDG_SetPrescaler(IWDG_Prescaler_4);

	IWDG_SetReload(10);



#ifdef POWER_SOURCE_DETECT
	/* ADC configuration */
	ADC_Configuration();
#endif



	/* USART Configuration */
	USART_Configuration(2,57600); //Initialize USART 2 device
	USART_BUFFER_CLEAR;
	Delay(70);

	/* USB Init */
	USB_Init();


	if(GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0) == SET )
		TxDString("Detect Pin!\r\n");

#ifdef DEBUG_ENABLE_BY_USART2
	TxDString("CM-9 SERISE SYSTEM INIT!\r\n");
#endif

	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET || GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0) == SET)
	{


		RCC_ClearFlag();
		while(bDeviceState != CONFIGURED);  //Wait until USB CDC is fully initialized
#ifdef DEBUG_ENABLE_BY_USART2
		TxDString("Start serial monitor\r\n");
#endif
		SerialMonitor();
	}

	//TxDString("Jump to application\r\n");

	/* Else case, execute user application that downloaded previously*/
	pFunction Jump_To_Application;
	u32 JumpAddress;

	JumpAddress =  *(u32 *)(FLASH_START_ADDRESS+ 4);
#ifdef DEBUG_ENABLE_BY_USART2
	TxDString("\r\n Go: 0x");
	TxDHex32(JumpAddress);
	TxDString("\r\n");
#endif
	Jump_To_Application = (pFunction) JumpAddress;
	Jump_To_Application();
	return 0;
}
void ClearTimeOutBuffer(void){

	gu32TimingCounter1ms =0;
}
u8 CheckTimeOut(void)
{
	// Check timeout
	// Return: 0 is false, 1 is true(timeout occurred)

	if( gu32TimingCounter1ms > 16){
		return 1;

	}else
		return 0;
}

//For Dynamixel
void Interrupt1ms(void)
{
	gu32TimingCounter1ms++;

}

void Timer_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* TIM2 configuration */
	TIM_TimeBaseStructure.TIM_Period = 79;
	//TIM_TimeBaseStructure.TIM_Period = 7;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Immediate load of TIM2 Precaler value */
	//       when interrupt resolution is 1us, SysTick->CTRL |= SysTick_Counter_Enable; in
	//       SysTick_CounterCmd(u32 SysTick_Counter) endlessly looping...
	TIM_PrescalerConfig(TIM2, 899, TIM_PSCReloadMode_Immediate);
	//TIM_PrescalerConfig(TIM2, 8, TIM_PSCReloadMode_Immediate);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);

	/* TIM2 IT enable */
	//       I don't know why enabling TIM_IT_CC1 makes while(gwTimingDelay != 0); endless loop...
	//TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);


	/* TIM2 enable counter */
	//TIM_Cmd(TIM2, ENABLE);


}
void ClearBuffer256(void)
{
	gbDXLReadPointer = gbDXLWritePointer = 0;
}

u32 dxl_get_baudrate(int baudnum, u8 option)
{
	if(option == 1){
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

	}else{
		return (2000000 / (baudnum + 1));
	}
}
/*
void PrintProtectStatus()
{
	int i;

	if( FLASH_GetReadOutProtectionStatus() == SET )
		USB_TxDString("\r\nRead Protection : SET");
	else
	{
		USB_TxDString("\r\nRead Protection : RESET... RP will be enabled now.");
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP|FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);
		FLASH_ReadOutProtection( ENABLE);
		FLASH_EnableWriteProtection(FLASH_WRProt_Pages0to3|FLASH_WRProt_Pages4to7|FLASH_WRProt_Pages8to11);
		FLASH_Lock();
		NVIC_GenerateSystemReset();
	}

	gwWriteProtectedPages = FLASH_GetWriteProtectionOptionByte();

	USB_TxDString("\r\nWrite Protection : ");

	if( gwWriteProtectedPages == 0xFFFFFFFF ) USB_TxDString("No");
	else
	{
		for( i = 0 ; i< 16 ; i++ )
		{
			if( !( gwWriteProtectedPages & (1<<i) ) )
			{
				USB_TxDString("0x");
				USB_TxDHex8(i*4);
				USB_TxDString("~");
				USB_TxDString("0x");
				USB_TxDHex8((i+1)*4-1);
				USB_TxDString(", ");
			}
		}
	}

}*/




void TxDByte(u8 dat)
{
	USART_ClearFlag( USART2, USART_FLAG_TC );//modified @new CM-900 2012-07-24
	USART_SendData(USART2,dat);
	while( USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET );
}
void TxD_Dec_U8(u8 bByte)
{
    u8 bTmp;
    bTmp = bByte/100;
    /*if(bTmp)*/ TxDByte( bTmp+'0');
    bByte -= bTmp*100;
    bTmp = bByte/10;
    /*if(bTmp)*/ TxDByte( bTmp+'0');
    bByte -= bTmp*10;
    TxDByte( bByte+'0');
}

void TxDString(char *str)
{
	int i;
	for(i=0; str[i] ; i++)
	{
		TxDByte(str[i]);
	}
}

void TxDHex8(u16 bSentData)
{
	u16 bTmp;
	
	bTmp = ((bSentData>>4)&0x000f) + (u8)'0';
	if(bTmp > '9') bTmp += 7;
	TxDByte(bTmp);
	
	bTmp = (bSentData & 0x000f) + (u8)'0';
	if(bTmp > '9') bTmp += 7;
	TxDByte(bTmp);
}

void TxDHex16(u16 wSentData)
{
	TxDHex8((wSentData>>8)&0x00ff );
	TxDHex8( wSentData&0x00ff);
}

void TxDHex32(u32 lSentData)
{
	TxDHex16((lSentData>>16)&0x0000ffff );
	TxDHex16( lSentData&0x0000ffff);
}


void USB_TxD_Dec_U8(u8 bByte)
{
    u8 bTmp;
    bTmp = bByte/100;
    /*if(bTmp)*/ USB_TxDByte( bTmp+'0');
    bByte -= bTmp*100;
    bTmp = bByte/10;
    /*if(bTmp)*/ USB_TxDByte( bTmp+'0');
    bByte -= bTmp*10;
    USB_TxDByte( bByte+'0');
}


void USB_TxDString(char *str)
{
	int i;
	for(i=0; str[i] ; i++)
	{
		USB_TxDByte(str[i]);
	}
}

void USB_TxDHex8(u16 bSentData)
{
	u16 bTmp;

	bTmp = ((bSentData>>4)&0x000f) + (u8)'0';
	if(bTmp > '9') bTmp += 7;
	USB_TxDByte(bTmp);

	bTmp = (bSentData & 0x000f) + (u8)'0';
	if(bTmp > '9') bTmp += 7;
	USB_TxDByte(bTmp);
}

void USB_TxDHex16(u16 wSentData)
{
	USB_TxDHex8((wSentData>>8)&0x00ff );
	USB_TxDHex8( wSentData&0x00ff);
}

void USB_TxDHex32(u32 lSentData)
{
	USB_TxDHex16((lSentData>>16)&0x0000ffff );
	USB_TxDHex16( lSentData&0x0000ffff);
}


u8 RxDBufferFromHost(void)
{
	return (u8)(gwpUSARTBuffer[(gwUSARTReadPtr++)&USART_BUFFER_SIZE]);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nTime)
{
	/* Enable the SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Enable);

	TimingDelay = nTime;

	while(TimingDelay != 0);

	/* Disable SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Disable);
	/* Clear SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Clear);
}

u8 gbCount =0;
#define IDE_COMMAND_LENGTH 10
extern volatile u16 USB_Rx_Cnt;
extern u8 USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
void SerialMonitor(void)
{
	u16 bTerminate;//,bRxData;
	u8 dxlBaudrate =0;
	char bpCommand[COMMAND_LENGTH];
	int i;
	for(i=0;i<COMMAND_LENGTH;i++){
		bpCommand[i]='\0';
	}
	u8 bRxData=0;
	u8 usbRxCount = 0;
	//PrintProtectStatus();

	GPIO_ResetBits(PORT_LED, PIN_LED);					// LED On modified @new CM-900 with jason 2012-07-24
	GPIO_ResetBits(CM904_PORT_LED, CM904_PIN_LED);		// LED On modified @new CM-904
	USART_BUFFER_CLEAR;

	bTerminate = 0;

	while(!bTerminate)
	{
		/*if(bDeviceState == CONFIGURED){
			USB_TxDString("\r\n -");
		}else{
			TxDString("\r\n -");
		}*/

		//GetCommandFromHost(gbpRxBuffer);
		while(!RXD_BUFFER_READY)
		{
			//wait
		}

	//	bRxData = RxDBufferFromHost();

/*
		if(gbCount >= IDE_COMMAND_LENGTH ){
			gbCount = 0;

		}
		else{
			bpCommand[gbCount++] = bRxData;
			if (gbCount == IDE_COMMAND_LENGTH)
				bpCommand[IDE_COMMAND_LENGTH] = '\0';

		}*/
#ifdef DEBUG_ENABLE_BY_USART2
	TxDString("USB_Rx_Cnt = ");	TxDHex8(USB_Rx_Cnt);	TxDString("\r\n");
	TxDString("USB_Rx_Buffer = ");	TxDString(USB_Rx_Buffer);	TxDString("\r\n");
#endif
	StringCopy(bpCommand,USB_Rx_Buffer);
	gbCount = USB_Rx_Cnt;
#ifdef DEBUG_ENABLE_BY_USART2
	TxDString("bpCommand = ");	TxDString(bpCommand);	TxDString("\r\n");
	TxDString("gbCount = ");	TxDHex8(gbCount);	TxDString("\r\n");
#endif
	USB_Rx_Cnt = 0;
	for(i=0;i < VIRTUAL_COM_PORT_DATA_SIZE;i++ ){
		 USB_Rx_Buffer[i] = '\0';

	}
		//bParaNum = StringProcess(bpCommand,ulpParameter,gbpRxBuffer);
		//bParaNum =1;
		//if(bParaNum != 0)
		if(/*gbCount == IDE_COMMAND_LENGTH &&*/gbCount > 3 && bpCommand[0] == 'A' && bpCommand[2] == '&')
		{
			//if(bParaNum > PARA_NUM) bParaNum = PARA_NUM;
			if( StringCompare(bpCommand,"AT&LD",5) )//if( StringCompare(bpCommand,"LD")|| StringCompare(bpCommand,"L") || bRxData == 'l' )
			{
				//TxDString("download\r\n");
				u32 EraseCounter = 0x00;
				vu32 NbrOfPage = 0x00;

				volatile FLASH_Status FLASHStatus;
				volatile TestStatus MemoryProgramStatus;

				vu32 tStartAddr;
				bRxData=0;

				FLASHStatus = FLASH_COMPLETE;
				MemoryProgramStatus = PASSED;

				gwAddressPointer = FLASH_START_ADDRESS;
				gwEndAddressPointer = FLASH_START_ADDRESS + 0xF800;

#ifdef DEBUG_ENABLE_BY_USART2
				TxDString("gwAddressPointer =");
				TxDHex32(gwAddressPointer);
				TxDString("\r\n");
				TxDString("gwEndAddressPointer =");
				TxDHex32(gwEndAddressPointer);
				TxDString("\r\n");
#endif
	/*			if( gwAddressPointer <  FLASH_START_ADDRESS || gwAddressPointer > FLASH_END_ADDRESS ||
						gwEndAddressPointer <  FLASH_START_ADDRESS || gwEndAddressPointer > FLASH_END_ADDRESS )
				{
					if(bDeviceState == CONFIGURED){
						USB_TxDString("\r\n Out of Range!\r\n");
					}else{
						TxDString("\r\n Out of Range!\r\n");
					}
					continue;
				}// 2012-05-15 jason added from CM-530 Bootloader 1.01*/

				tStartAddr = gwAddressPointer;
				/*Init global variable related to flash download*/
				gwRxTotalCount = gwCalculatedCheckSum = 0;


				/* Unlock the Flash Program Erase controller */
				FLASH_Unlock();
				gbIsFlashLock = FALSE;

				/* Define the number of page to be erased */
				NbrOfPage = ((gwEndAddressPointer - gwAddressPointer) / FLASH_PAGE_SIZE );  // 2012-05-15 jason added from CM-530 Bootloader 1.01
				/*if ((FLASH_END_ADDRESS - gwAddressPointer) & 0x3ff)
				  NbrOfPage++;*/

				/* Clear All pending flags */
				FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

				// 2012-05-15 jason added from CM-530 Bootloader 1.01
				//TxDString("\r\nErasing....     ");

				/* Erase the FLASH pages */
				for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE) ; EraseCounter++)
				{
					//u16 percent;
					if( (gwAddressPointer + (FLASH_PAGE_SIZE * EraseCounter)) > 0x0800F000)
						break;
					FLASHStatus = FLASH_ErasePage(gwAddressPointer + (FLASH_PAGE_SIZE * EraseCounter));
					TxDHex32(gwAddressPointer + (FLASH_PAGE_SIZE * EraseCounter));
					//USB_TxDString("\b\b\b\b");
					//percent = (u16)EraseCounter * 100 / (u16)NbrOfPage;
					//USB_TxD_Dec_U8(percent);
					//USB_TxDString("\%");
				}
				//USB_TxDString("\b\b\b\b");
				//USB_TxD_Dec_U8(100);
				//USB_TxDString("\%");
				//TxDString("complete!\r\n");
				if( FLASHStatus != FLASH_COMPLETE )
				{
					FLASH_Lock();
					continue;
				}
				//USB_TxDString("\r\n Write Address : ");USB_TxDHex32(gwAddressPointer);
				USB_TxDString("Ready..\n");
				gbFlashDownloadStart = TRUE;
				TIM_Cmd(TIM2, ENABLE);
				Delay(100); // some delay is needed because PC have some time to prepare data.

				while(1){
					//wait until flash-download is finished
					//flash-download code is located EP3_OUT_Callback() in usb_endp.c
					//getting checksum data from Host PC(IDE)
					if(USB_Rx_Cnt != 0x0)
						usbRxCount = USB_Rx_Cnt;
					if(CheckTimeOut()){
						gbFlashDownloadStart = FALSE;
						ClearTimeOutBuffer();
						TIM_Cmd(TIM2, DISABLE);
						//TxDString("\r\n Flash writing finished\r\n");
						USART_BUFFER_CLEAR
						break;
					}

				}
#ifdef DEBUG_ENABLE_BY_USART2
				TxDString("usbRxCount = ");	TxDHex8(usbRxCount);	TxDString("\r\n");
				TxDString("USB_Rx_Buffer = ");	TxDByte(USB_Rx_Buffer[usbRxCount-1]);	TxDString("\r\n");
#endif
				gwReceivedCheckSumFromHost = USB_Rx_Buffer[usbRxCount-1];
				gwCalculatedCheckSum = gwCalculatedCheckSum - gwReceivedCheckSumFromHost;
				/*while(!RXD_BUFFER_READY); //wait until check-sum data is received from PC
				gwReceivedCheckSumFromHost = RxDBufferFromHost();
				TxDString("\r\n gwReceivedCheckSumFromHost = ");	TxDHex32(gwReceivedCheckSumFromHost);	TxDString("\r\n");
				*/
				/* Lock again the Flash Program Erase controller */
				FLASH_Lock();
				gbIsFlashLock = TRUE;
			//	TxDString("\r\n Size:0X");TxDHex16(gwRxTotalCount);
			//	TxDString("  Checksum:"); TxDHex16((u8)(gwCalculatedCheckSum&0xff));TxDString("-");TxDHex16(gwReceivedCheckSumFromHost);
				if((gwCalculatedCheckSum&0xff) == gwReceivedCheckSumFromHost || gwReceivedCheckSumFromHost == '*' ){
					USB_TxDString("Success..\n");
				}
				else
					USB_TxDString("Fail..\n");
			//WDTCR = 0x08;
				Delay(100);

			}
			else if(StringCompare(bpCommand,"AT&GO",5))//else if(StringCompare(bpCommand,"GO")|| StringCompare(bpCommand,"G") || bRxData == 'g' )
			{

				//TxDString("Jump to user application!\r\n");
				pFunction Jump_To_Application;
				u32 JumpAddress;

				/*if(bParaNum == 2){
					JumpAddress =  *(u32 *)(ulpParameter[0] + 4);
				}
				else*/
				{
					JumpAddress =  *(u32 *)(FLASH_START_ADDRESS + 4);
				}
				//NVIC_SetVectorTable(NVIC_VectTab_FLASH, ((JumpAddress-4)&0xFFFF) );

				Jump_To_Application = (pFunction) JumpAddress;

			/*	if(bDeviceState == CONFIGURED){
					USB_TxDString("\r\n Go: ");
					USB_TxDHex32(JumpAddress);
					USB_TxDString("\r\n");
				}*/

				UsbVcpDisconnect();
#ifdef DEBUG_ENABLE_BY_USART2
				TxDString("USB Power Off!\r\n");
#endif
				Delay(100);
				Jump_To_Application();
			}
			else if(StringCompare(bpCommand,"AT&RST",6)){
#ifdef DEBUG_ENABLE_BY_USART2
				TxDString("system reset \r\n ");
#endif
				NVIC_GenerateSystemReset();
			}
			else if(StringCompare(bpCommand,"AT&TOSS",7)){
#ifdef DEBUG_ENABLE_BY_USART2
				TxDString("TOSS Mode for dynamixel \r\n ");
#endif
				USB_TxDString("TOSS MODE OK\r\n ");
				if(bpCommand [7] == '=' || bpCommand [7] == '*' ){

					if(gbCount == 9){
						if(bpCommand[8] > 47 && bpCommand[8] < 58){
							dxlBaudrate = bpCommand[8] - 48;
						}

					}else if(gbCount == 10){
						if(bpCommand[8] > 47 && bpCommand[8] < 58 && bpCommand[9] > 47 && bpCommand[9] < 58 ){
							dxlBaudrate = (bpCommand[8] - 48)*10 + (bpCommand[9] - 48);
						}

					}else{
					//nothing...
					}
#ifdef DEBUG_ENABLE_BY_USART2
					//TxDString("Dxl baud rate =  ");TxD_Dec_U8(dxlBaudrate);TxDString("\r\n ");
#endif
					if(bpCommand[7] == '='){
					// Dynamixel 1.0
						USART_Configuration(1, dxl_get_baudrate(dxlBaudrate,0));
						//TxDString("Dxl baud rate =  ");TxDHex32(dxl_get_baudrate(dxlBaudrate,0));TxDString("\r\n ");
					}else{
					// Dynamixel 2.0
						USART_Configuration(1, dxl_get_baudrate(dxlBaudrate,1));
					}
					USART_BUFFER_CLEAR
					gbDXLWritePointer = gbDXLReadPointer = 0;
					USB_Rx_Cnt = 0;
					while(1)
					{

						if(USB_Rx_Cnt > 0)//if(gwUSARTReadPtr != gwUSARTWritePtr) //USB -> DXL
						{
#ifdef DEBUG_ENABLE_BY_USART2
							TxDString("USB_Rx_Cnt = ");	TxDHex8(USB_Rx_Cnt);	TxDString("\r\n");
							TxDString("USB_Rx_Buffer = ");	TxDString(USB_Rx_Buffer);	TxDString("\r\n");
#endif
							if(USB_Rx_Buffer[0] == '!' && USB_Rx_Buffer[1] == '!' && USB_Rx_Buffer[2] == '!'){
								//TxDString("Escaped!!\r\n");
								USART_BUFFER_CLEAR
								USB_Rx_Cnt = 0;
								gbDXLWritePointer = gbDXLReadPointer = 0;
								break;
							}
							GPIO_SetBits(GPIOB, GPIO_Pin_5);	// TX Enable
							for(i=0; i < USB_Rx_Cnt; i++){
								USART_SendData(USART1,(u8)USB_Rx_Buffer[i]);
								while( USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET );
							}
							USB_Rx_Cnt = 0;
							GPIO_ResetBits(GPIOB, GPIO_Pin_5);	// RX Enable

						}

						if(gbDXLWritePointer != gbDXLReadPointer){
							while(gbDXLWritePointer != gbDXLReadPointer)//DXL -> USB
							{
								USB_TxDByte(gbpDXLDataBuffer[gbDXLReadPointer++]);
								gbDXLReadPointer = gbDXLReadPointer & USART_BUFFER_SIZE;
							}
						}
					}
				}
			}
			else if(StringCompare(bpCommand,"AT&NAME",7)){
				USB_TxDString("CM-904\n");
			}
			else{
				TxDString("No IDE Command!\r\n");
			}

		}else{
			if(bpCommand[0] == 'A' && bpCommand[1] == 'T'){
				USB_TxDString("OK\n");
			}
		}
		gbCount = 0;
		for(i=0;i<COMMAND_LENGTH;i++){
				bpCommand[i]='\0'; //clear command buffer
		}
		USART_BUFFER_CLEAR;


	}//while()

}

//no need to use the below func.
/*
u16 StringProcess(char *bpCommand, u32 *wpPara, char *bpString)
{
	u16 bCount, bParaCount;
	u16 bPtr;
	bCount = 0;
	Command
	while(bpString[bCount] == ' ') bCount++; remove space
	while(bCount < COMMAND_BUFFER_SIZE && bpString[bCount] != '\0')
	{
		bPtr = 0;
		while('0' <= bpString[bCount]&&bpString[bCount] <='z' && bCount < COMMAND_LENGTH)
		{
			if('a' <= bpString[bCount]&&bpString[bCount] <= 'z') (bpString[bCount])&=0x5f; //ToUpper
			bpCommand[bPtr++] = bpString[bCount];
			bCount++;
		}
		bpCommand[bPtr] = '\0';
		break;
	}
	if(bpCommand[0] == '\0') return 0;

	for(bParaCount = 0; bParaCount < PARA_NUM; bParaCount++)
	{
		while(bpString[bCount] == ' ') bCount++; remove space
		if(bpString[bCount] == '\0') return(bParaCount+1);  +1 mean bpCommand
		wpPara[bParaCount] = 0;
		while(bCount < COMMAND_BUFFER_SIZE)
		{
			if(bpString[bCount] == '\0') return(bParaCount+2);  +2 mean bpCommand+lastnum
			if( bpString[bCount] == ' ')
			{
				bpString[bCount] = ' ';
				break;
			}
			else
			{
				if('0' <= bpString[bCount] && bpString[bCount] <= '9')
				{
					wpPara[bParaCount] <<= 4;
					wpPara[bParaCount] += bpString[bCount] -'0';
				}
				else if('A' <= bpString[bCount] && bpString[bCount] <= 'z')
				{
					bpString[bCount] &= 0x5f;
					wpPara[bParaCount] <<= 4;
					wpPara[bParaCount] += bpString[bCount] -'0'-7;
				}
				bCount++;
			}
		}
	}
	return (bParaCount+1);
}
*/

/*

u16 GetCommandFromHost(char *bpStringPointer)
{
	u16 bRxData,bCount;
	static char bpBackupBuffer[COMMAND_BUFFER_SIZE];
	u16 bTerminate = 1;

	bCount = 0;
	#define CTRL_A 	1
	#define ESC_KEY 0x1b
    #define LF  	10
    #define CR  	13
    #define BS  	8
	while(bTerminate)
	{
		while(!RXD_BUFFER_READY)
		{
			//wait
		}
		bRxData = RxDBufferFromHost();
//#ifdef DEBUG_ENABLE_BY_USART2
		TxDString("bRxData = ");	TxDByte(bRxData);	TxDString("\r\n");
//#endif
		switch(bRxData)
		{
			case 0:
			case ESC_KEY:
				break;

			case CR:
			case LF:
				TxDString("Enter detected\r\n");
				bpStringPointer[bCount] = '\0';
				StringCopy(bpBackupBuffer,bpStringPointer);
				return(1);
				break;

			case BS:
				if(bCount > 0)
				{
					bCount--;
					USB_TxDByte(BS);
					USB_TxDByte(' ');
					USB_TxDByte(BS);
				}
				break;

			default:
				if(bCount < COMMAND_BUFFER_SIZE)
				{
					bpStringPointer[bCount]= bRxData;
					if(bDeviceState == CONFIGURED){
						USB_TxDByte(bRxData); //Typed character send again
					}else{
						TxDByte(bRxData); //Typed character send again
					}
					bCount++;
				}
				break;
		}
	}
	return 0;
}
*/

u16 StringCompare(char *bpA, char *bpB, int limit)
{
	u16 wCount;
	wCount = 0;
	while(bpA[wCount] == bpB[wCount])
	{
		wCount++;
		if ( wCount >= limit)
			return wCount;
		if((bpA[wCount] == '\0' && bpB[wCount] == '\0') || wCount > COMMAND_BUFFER_SIZE-1 ) return wCount;
	}
	return 0;
}

u16 StringCopy(char *bpDst, char *bpSrc)
{
    u16 bCount;
    for(bCount =0; bCount < IDE_COMMAND_LENGTH/*COMMAND_BUFFER_SIZE*/; bCount++)
    {
        bpDst[bCount] = bpSrc[bCount];
        if(bpSrc[bCount] == '\0') break;
    }
    return bCount;
}
#ifdef USE_EEPROM_EMULATOR
u16 EEPROM_Read( u32 Offset )
{
	u16* Adr;
	Adr = (u16*)(EEPROM_START_ADDRESS + (Offset<<1));
	return *Adr;
}

void EEPROM_Write( u32 Offset, u16 Data )
{
	volatile FLASH_Status FLASHStatus;
	u32 Adr;
	u16 cnt;
	u16 Buffer[512];

	Adr = EEPROM_START_ADDRESS + (Offset<<1);

	if( (Data != EEPROM_Read(Offset)) && (Offset<512) )
	{
		for( cnt=0; cnt<512; cnt++ )
		{
			Buffer[cnt] = EEPROM_Read(cnt);
		}
		Buffer[Offset] = Data;

		FLASH_Unlock();
		/* Clear All pending flags */
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

		if( (Data==0) || (EEPROM_Read(Offset)==0xFFFF) )
		{
			FLASHStatus = FLASH_ProgramHalfWord( Adr, Data );
		}
		else
		{		// Erase
			/* Erase the FLASH pages */
			FLASHStatus = FLASH_ErasePage( EEPROM_START_ADDRESS);

			Adr = EEPROM_START_ADDRESS;

			for( cnt=0; cnt<512; cnt++ )
			{
				if( Buffer[cnt] != 0xFFFF ) FLASHStatus = FLASH_ProgramHalfWord( Adr, Buffer[cnt] );
				Adr += 2;
			}
		}

		FLASH_Lock();
	}
}

void EEPROM_Clear( void )
{
	volatile FLASH_Status FLASHStatus;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASHStatus = FLASH_ErasePage( EEPROM_START_ADDRESS);
	FLASH_Lock();
}
#endif
void USART_Configuration(u8 PORT, u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;

	USART_StructInit(&USART_InitStructure);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	if( PORT == 1 )
	{
		USART_DeInit(USART1);
		Delay(10);
		/* Configure the USART1 */
		USART_Init(USART1, &USART_InitStructure);

		/* Enable USART1 Receive and Transmit interrupts */
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		//USART_ITConfig(USART1, USART_IT_TC, ENABLE);

		/* Enable the USART1 */
		USART_Cmd(USART1, ENABLE);
	}

	else if( PORT == 2 )
	{
		USART_DeInit(USART2);
		Delay(10);
		/* Configure the UART5 */
		USART_Init(USART2, &USART_InitStructure);

		/* Enable UART5 Receive and Transmit interrupts */
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

		/* Enable the UART5 */
		USART_Cmd(USART2, ENABLE);
	}

	else if( PORT == 3 )
	{

		USART_DeInit(USART3);
		Delay(10);
		/* Configure the USART3 */
		USART_Init(USART3, &USART_InitStructure);

		/* Enable USART3 Receive and Transmit interrupts */
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		//USART_ITConfig(USART3, USART_IT_TC, ENABLE);

		/* Enable the USART3 */
		USART_Cmd(USART3, ENABLE);
	}
}
#if 0
void USART_Configuration(u32 nBaud)
{
//	USART_InitTypeDef USART_InitStructure;

	/* USART2 configuration ------------------------------------------------------*/
	/* USART2 configured as follow:
		- BaudRate = 57600 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	*/
	
	//USART_StructInit( &USART_InitStructure );
	
	USART_InitStructure.USART_BaudRate = nBaud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_DeInit(USART2);//modified @new CM-900
	Delay(10);

	USART_Init(USART2, &USART_InitStructure);
	
	/* Enable USART2 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Enable the USART2 */
	USART_Cmd(USART2, ENABLE);
}
#endif
/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
	RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Select USBCLK source */
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
	/* Enable peripheral clocks --------------------------------------------------*/
	//RCC_ADCCLKConfig(RCC_PCLK2_Div6);    // added 2012-05-10 jason

	/* Enable USART1, GPIOA, GPIOB, and AFIO clocks */ //add RCC_APB2Periph_GPIOC for USB_DISCONNECT pin by sm.lee 2012-08-13
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_USART1 |
							RCC_APB2Periph_GPIOB  |
							RCC_APB2Periph_GPIOA  |
							RCC_APB2Periph_GPIOC  |
#ifdef POWER_SOURCE_DETECT
							RCC_APB2Periph_ADC1   |
							RCC_APB2Periph_ADC2   |
#endif
						    RCC_APB2Periph_AFIO,
						    ENABLE);
	//add RCC_APB1Periph_USART2 clock @ new CM-900 with jason 2012-07-24
  	RCC_APB1PeriphClockCmd(
  							RCC_APB1Periph_USB    |
  							RCC_APB1Periph_TIM2   |
  						    RCC_APB1Periph_USART2 |
  						    RCC_APB1Periph_PWR ,
  						    ENABLE);

	//PWR_BackupAccessCmd(ENABLE);
}



/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{




	GPIO_InitTypeDef GPIO_InitStructure;


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//긴급 복구 모드
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* Configure USART2 Rx(Alter.F) (PA.3) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//modified by sm.lee 2012-07-24
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Tx(Alter.F) (PA.2) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//modified by sm.lee 2012-07-24
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

#ifdef POWER_SOURCE_DETECT
	/* Configure USB Power Sensing PA.7*/
	GPIO_InitStructure.GPIO_Pin =  PIN_USB_POWER;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(PORT_USB_POWER, &GPIO_InitStructure);
#endif
/* Configure LED (PB.2) as output push-pull */
	GPIO_InitStructure.GPIO_Pin = PIN_LED;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORT_LED, &GPIO_InitStructure);

/* Configure LED (PB.2) as output push-pull */
	GPIO_InitStructure.GPIO_Pin = CM904_PIN_LED;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CM904_PORT_LED, &GPIO_InitStructure);

/* Configure USB Disconnect pin (PA.8) as output push-pull */
	GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;/*test for USB disconnect*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;// GPIO_Speed_50MHz;//GPIO_Speed_10MHz; PC13~15 is limited their' speed to 2Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(USB_DISCONNECT_PORT, &GPIO_InitStructure);

	GPIO_SetBits(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN);//USB Pull-up must be disabled(go to High) during system power on

	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_5; //DXL DIR
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //DXL RXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //DXL TXD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB, GPIO_Pin_5);	// TX Disable // RX Enable

	GPIO_SetBits(PORT_LED, PIN_LED);		// LED Off
	GPIO_SetBits(CM904_PORT_LED, CM904_PIN_LED);		// 904 LED Off

	/* Configure USART1 Remap enable */
	GPIO_PinRemapConfig( GPIO_Remap_USART1, ENABLE);
	/* If use both DXL Enable_TX/RX pin and JTAG Debug/Downloading, need GPIO_Remap_SWJ_NoJTRST option */
	GPIO_PinRemapConfig( GPIO_Remap_SWJ_NoJTRST,ENABLE);//GPIO_Remap_SWJ_Disable, ENABLE);


}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM  
/* Set the Vector Table base location at 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
/*
 * Set the Vector Table base location at 0x08000000 in Boot-loader case
 * Set the Vector Table base location at 0x08003000 in Application case
 */
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);   
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif


/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);


	/* Enable the USB RX0 Interrupt */
	 NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;//USB_LP_CAN1_RX0_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);

	// Enable the USART1 Interrupt for DXL
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	 /* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;// USART interrupt when connect to Zigbee USART
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
#ifdef POWER_SOURCE_DETECT
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	ADC_StructInit(&ADC_InitStructure);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channels configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1 , ADC_SampleTime_239Cycles5);
	//ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	/* Enable ADC1 DMA */
	//ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */
	/* Check the end of ADC1 reset calibration register */
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));


	/* Start ADC1 calibration */
	/* Check the end of ADC1 calibration */
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));


	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
#endif

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
