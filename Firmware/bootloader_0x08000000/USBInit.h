/* Copyright (C) 2012 ROBOTIS, Co., Ltd.
 *
 * @File: USB_Init.h
 * @Brief : USB init header file for cm-9 serise bootloader
 * @Date : 2012. 08. 27
 * @Author : sm6787@robotis.com
 */
#ifndef USB_INIT_H_
#define USB_INIT_H_

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "lib/USB_lib/inc/usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_istr.h"
//#include "stm32f10x_it.h"

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/

#define EP_NUM                          (4)


/* Exported types ------------------------------------------------------------*/
#ifdef USE_USB_POWER_MANAGEMENT
typedef enum _RESUME_STATE
{
  RESUME_EXTERNAL,
  RESUME_INTERNAL,
  RESUME_LATER,
  RESUME_WAIT,
  RESUME_START,
  RESUME_ON,
  RESUME_OFF,
  RESUME_ESOF
} RESUME_STATE;

struct
{
  __IO RESUME_STATE eState;
  __IO u8 bESOFcnt;
}ResumeS;
#endif

typedef enum _DEVICE_STATE
{
  UNCONNECTED,
  ATTACHED,
  POWERED,
  SUSPENDED,
  ADDRESSED,
  CONFIGURED
} DEVICE_STATE;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef USE_USB_POWER_MANAGEMENT
void Suspend(void);
void Resume_Init(void);
void Resume(RESUME_STATE eResumeSetVal);
#endif

RESULT PowerOn(void);
RESULT PowerOff(void);

void UsbVcpConnect(void);
void UsbVcpDisconnect(void);

#ifdef DEBUG_ENABLE_BY_USART2
extern void TxDString(char *str);
#endif

#ifdef USE_USB_POWER_MANAGEMENT
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
#endif

void USB_Cable_Config (FunctionalState NewState);
void Get_SerialNum(void);
//void USB_OTG_BSP_uDelay (const u32 usec);

extern void Delay(u32 nTime);




#endif /* USB_INIT_H_ */
