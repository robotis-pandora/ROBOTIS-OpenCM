/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : All processing related to Virtual Com Port Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "USBInit.h"
#include "usb_conf.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Request = 0;

extern __IO u32 bDeviceState;

LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* no. of bits 8*/
  };

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    Virtual_Com_Port_init,
    Virtual_Com_Port_Reset,
    Virtual_Com_Port_Status_In,
    Virtual_Com_Port_Status_Out,
    Virtual_Com_Port_Data_Setup,
    Virtual_Com_Port_NoData_Setup,
    Virtual_Com_Port_Get_Interface_Setting,
    Virtual_Com_Port_GetDeviceDescriptor,
    Virtual_Com_Port_GetConfigDescriptor,
    Virtual_Com_Port_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Virtual_Com_Port_GetConfiguration,
    Virtual_Com_Port_SetConfiguration,
    Virtual_Com_Port_GetInterface,
    Virtual_Com_Port_SetInterface,
    Virtual_Com_Port_GetStatus,
    Virtual_Com_Port_ClearFeature,
    Virtual_Com_Port_SetEndPointFeature,
    Virtual_Com_Port_SetDeviceFeature,
    Virtual_Com_Port_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (u8*)Virtual_Com_Port_DeviceDescriptor,
    VIRTUAL_COM_PORT_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (u8*)Virtual_Com_Port_ConfigDescriptor,
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(u8*)Virtual_Com_Port_StringLangID, VIRTUAL_COM_PORT_SIZ_STRING_LANGID},
    {(u8*)Virtual_Com_Port_StringVendor, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR},
    {(u8*)Virtual_Com_Port_StringProduct, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT},
    {(u8*)Virtual_Com_Port_StringSerial, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Virtual_Com_Port_init.
* Description    : Virtual COM Port Mouse init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();

  pInformation->Current_Configuration = 0;

  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
  //TxDString("Virtual_Com_Port_init\r\n");
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Reset
* Description    : Virtual_Com_Port Mouse reset routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Reset(void)
{
  /* Set Virtual_Com_Port DEVICE as not configured */
  pInformation->Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = Virtual_Com_Port_ConfigDescriptor[7];

  /* Set Virtual_Com_Port DEVICE with the default Interface*/
  pInformation->Current_Interface = 0;

#ifdef STM32F10X_CL     
  /* EP0 is already configured by USB_SIL_Init() function */
  
  /* Init EP1 IN as Bulk endpoint */
  OTG_DEV_EP_Init(EP1_IN, OTG_DEV_EP_TYPE_BULK, VIRTUAL_COM_PORT_DATA_SIZE);
  
  /* Init EP2 IN as Interrupt endpoint */
  OTG_DEV_EP_Init(EP2_IN, OTG_DEV_EP_TYPE_INT, VIRTUAL_COM_PORT_INT_SIZE);

  /* Init EP3 OUT as Bulk endpoint */
  OTG_DEV_EP_Init(EP3_OUT, OTG_DEV_EP_TYPE_BULK, VIRTUAL_COM_PORT_DATA_SIZE);  
#else 

  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_BULK);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxStatus(ENDP1, EP_TX_NAK);
  SetEPRxStatus(ENDP1, EP_RX_DIS);

  /* Initialize Endpoint 2 */
  SetEPType(ENDP2, EP_INTERRUPT);
  SetEPTxAddr(ENDP2, ENDP2_TXADDR);
  SetEPRxStatus(ENDP2, EP_RX_DIS);
  SetEPTxStatus(ENDP2, EP_TX_NAK);

  /* Initialize Endpoint 3 */
  SetEPType(ENDP3, EP_BULK);
  SetEPRxAddr(ENDP3, ENDP3_RXADDR);
  SetEPRxCount(ENDP3, VIRTUAL_COM_PORT_DATA_SIZE);
  SetEPRxStatus(ENDP3, EP_RX_VALID);
  SetEPTxStatus(ENDP3, EP_TX_DIS);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
#endif /* STM32F10X_CL */

  bDeviceState = ATTACHED;
  //TxDString("Virtual_Com_Port_Reset\r\n");
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  //TxDString("Virtual_Com_Port_SetConfiguration\r\n");
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
  //TxDString("Virtual_Com_Port_SetDeviceAddress\r\n");
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_In.
* Description    : Virtual COM Port Status In Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_In(void)
{
  if (Request == SET_LINE_CODING)
  {

    //USART_Config();
    //config changed
    Request = 0;
  }

}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_Out
* Description    : Virtual COM Port Status OUT Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_Out(void)
{
	//TxDString("Virtual_Com_Port_Status_Out Request ");TxDHex8(Request);  TxDString("\r\n");
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Data_Setup
* Description    : handle the data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
int comstatetemp=0;
RESULT Virtual_Com_Port_Data_Setup(u8 RequestNo)
{
//#ifdef DEBUG_ENABLE_BY_USART2
 // TxDString("Virtual_Com_Port_Data_Setup ->RequestNo = ");  TxDHex8(RequestNo);  TxDString("\r\n");
//#endif
  u8    *(*CopyRoutine)(u16);
  CopyRoutine = NULL;

  if (RequestNo == GET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = Virtual_Com_Port_GetLineCoding;
    }
  }
  else if (RequestNo == SET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = Virtual_Com_Port_SetLineCoding;
    }
    Request = SET_LINE_CODING;
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_NoData_Setup.
* Description    : handle the no data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
/*extern bool gbFlashDownloadStart;
extern volatile u16 gwUSARTReadPtr, gwUSARTWritePtr;*/
RESULT Virtual_Com_Port_NoData_Setup(u8 RequestNo)
{
	//u8 new_signal = pInformation->USBwValues.bw.bb0 & (0x3);
	//u8 new_signal2 = pInformation->USBwValues.bw.bb1 & (0x3);
	//TxDString("new_signal_L ");TxDHex8(new_signal);TxDString("\r\n");
	//TxDString("new_signal_H ");TxDHex8(new_signal2);TxDString("\r\n");
/*	if(new_signal == 0x2){
		//IWDG_Enable();
	}
	else if(new_signal == 0x1){
		gbFlashDownloadStart = FALSE;
		gwUSARTReadPtr=0;
		gwUSARTWritePtr=0;
	}
	else if(new_signal == 0x0){
		gbFlashDownloadStart = TRUE;

	}
	(gbFlashDownloadStart ? TxDString("gbFlashDownloadStart is true\r\n") : TxDString("gbFlashDownloadStart is false\r\n"));*/
  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
  {
    if (RequestNo == SET_COMM_FEATURE)
    {
      return USB_SUCCESS;
    }
    else if (RequestNo == SET_CONTROL_LINE_STATE)
    {
      return USB_SUCCESS;
    }
  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
u8 *Virtual_Com_Port_GetDeviceDescriptor(u16 Length)
{
	//TxDString("Virtual_Com_Port_GetDeviceDescriptor\r\n");
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetConfigDescriptor.
* Description    : get the configuration descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
u8 *Virtual_Com_Port_GetConfigDescriptor(u16 Length)
{
	//TxDString("Virtual_Com_Port_GetConfigDescriptor\r\n");
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
u8 *Virtual_Com_Port_GetStringDescriptor(u16 Length)
{
  //TxDString("Virtual_Com_Port_GetStringDescriptor\r\n");
  u8 wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Get_Interface_Setting.
* Description    : test the interface and the alternate setting according to the
*                  supported one.
* Input1         : u8: Interface : interface number.
* Input2         : u8: AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
RESULT Virtual_Com_Port_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
  //TxDString("Virtual_Com_Port_Get_Interface_Setting\r\n");
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 1)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetLineCoding.
* Description    : send the linecoding structure to the PC host.
* Input          : Length.
* Output         : None.
* Return         : Inecoding structure base address.
*******************************************************************************/
u8 *Virtual_Com_Port_GetLineCoding(u16 Length)
{
//#ifdef DEBUG_ENABLE_BY_USART2
	//TxDString("GetLineCoding  bitrate = ");TxDHex32(linecoding.bitrate);  TxDString("\r\n");
//#endif
	//if(linecoding.bitrate == 0x4B0){
	//		  TxDString("System reset operates!\r\n");
	//		  comstatetemp = 1000;
			  //Delay(100);
			  //UsbVcpDisconnect();
			  //Delay(5000);
			  //NVIC_GenerateSystemReset();
			  //NVIC_GenerateCoreReset();
	//}

  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(u8 *)&linecoding;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetLineCoding.
* Description    : Set the linecoding structure fields.
* Input          : Length.
* Output         : None.
* Return         : Linecoding structure base address.
*******************************************************************************/
u8 *Virtual_Com_Port_SetLineCoding(u16 Length)
{
#ifdef DEBUG_ENABLE_BY_USART2
	TxDString("SetLineCoding bitrate = ");TxDHex32(linecoding.bitrate);  TxDString("\r\n");
#endif
   if(linecoding.bitrate == 0x4B0){
		  //TxDString("System reset operates!\r\n");
		  comstatetemp = 100;
	}
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }

  return(u8 *)&linecoding;
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

