/******************* (C) COPYRIGHT 2011 STMicroelectronics **************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx.h"
#include "usb_type.h"
#include "flash.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
/* Define the size of the sectors to be used */
#define FLASH_PAGE_SIZE               (u32)0x400  /* Page size = 2KByte */

/* Device voltage range supposed to be [2.7V to 3.6V], the operation will 
   be done by word  */
//#define VOLTAGE_RANGE           (uint8_t)VoltageRange_3


#define EEPROM_START_ADDRESS  ((u32)0x0800F800)

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS    ((u32)(EEPROM_START_ADDRESS + 0x0000))
#define PAGE0_END_ADDRESS     ((u32)(EEPROM_START_ADDRESS + (FLASH_PAGE_SIZE - 1)))
//#define PAGE0_ID               FLASH_Sector_2

#define PAGE1_BASE_ADDRESS    ((u32)(EEPROM_START_ADDRESS + FLASH_PAGE_SIZE))
#define PAGE1_END_ADDRESS     ((u32)(EEPROM_START_ADDRESS + (2 * FLASH_PAGE_SIZE - 1)))
//#define PAGE1_ID               FLASH_Sector_3

/* Used Flash pages for EEPROM emulation */
#define PAGE0                 ((u16)0x0000)
#define PAGE1                 ((u16)0x0001)

#define STATUS_EEPROM_EMPTY		0xFFFF
#define STATUS_EEPROM_USE		0xABAB
#define STATUS_EEPROM_FULL		0x0000

#define MAX_VIRTUAL_ADDRESS		(510)


u16 EEPROM_Read( u16 wVirtualAddrss );
void EEPROM_Write( u16 wVirtualAddrss , u16 wData );
void EEPROM_INIT(void);
void EEPROM_WRITE_PONTER_SETUP(u8 Page);
void EEPROM_COPY_PAGE(u8 DestPage, u8 SrcPage);
void EEPROM_ERASE(u8 Page);

#ifdef __cplusplus
}
#endif

#endif /* __EEPROM_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
