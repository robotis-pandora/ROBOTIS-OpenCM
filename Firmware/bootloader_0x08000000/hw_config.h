/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.h
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H


/* Includes ------------------------------------------------------------------*/
//#include "usb_type.h"
#include "lib/USB_lib/inc/usb_lib.h"
#include "usb_prop.h"
#include "usb_conf.h"
#include "usb_desc.h"

#define FLASH_START_ADDRESS ((u32)0x8003000)
//#define FLASH_END_ADDRESS	((u32)0x0800FFFF)
#define FLASH_PAGE_SIZE    	((u16)0x400)

#define FLASH_END_ADDRESS	((u32)0x0800FBFF)
#define EEPROM_START_ADDRESS	((u32)0x0800FC00)

#define USB_DISCONNECT_PORT                 GPIOC
#define USB_DISCONNECT_PIN                  GPIO_Pin_13

#define PORT_LED 							GPIOB
#define PIN_LED  							GPIO_Pin_2

#define CM904_PORT_LED 							GPIOB
#define CM904_PIN_LED  							GPIO_Pin_9


#ifdef POWER_SOURCE_DETECT
#define PORT_USB_POWER 						GPIOA
#define PIN_USB_POWER  						GPIO_Pin_7
#endif


/* Exported functions ------------------------------------------------------- */

/* External variables --------------------------------------------------------*/

#endif  /*__HW_CONFIG_H*/
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
