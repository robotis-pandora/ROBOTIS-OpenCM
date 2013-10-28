/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file flash.h
 * @brief STM32 Medium and high density Flash register map and setup
 * routines
 */

#include "libpandora_types.h"

#ifndef _FLASH_H_
#define _FLASH_H_

#ifdef __cplusplus
extern "C"{
#endif



/**
 * IO definitions
 *
 * define access restrictions to peripheral registers
 */

/** Flash register map type */
typedef struct flash_reg_map {
    __io uint32 ACR;            /**< Access control register */
    __io uint32 KEYR;           /**< Key register */
    __io uint32 OPTKEYR;        /**< OPTKEY register */
    __io uint32 SR;             /**< Status register */
    __io uint32 CR;             /**< Control register */
    __io uint32 AR;             /**< Address register */
    __io uint32 OBR;            /**< Option byte register */
    __io uint32 WRPR;           /**< Write protection register */
} flash_reg_map;

/** Flash register map base pointer */
#define FLASH_BASE                      ((struct flash_reg_map*)0x40022000)

/**
  * @brief  FLASH Status
  */

typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;


/*
 * Register bit definitions
 */

/** @defgroup FLASH_Flags
  * @ [ROBOTIS] added
  */

#define FLASH_FLAG_BSY                 ((uint32)0x00000001)  /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP                 ((uint32)0x00000020)  /*!< FLASH End of Operation flag */
#define FLASH_FLAG_PGERR               ((uint32)0x00000004)  /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR            ((uint32)0x00000010)  /*!< FLASH Write protected error flag */
#define FLASH_FLAG_OPTERR              ((uint32)0x00000001)  /*!< FLASH Option Byte error flag */

#define FLASH_FLAG_BANK1_BSY                 FLASH_FLAG_BSY       /*!< FLASH BANK1 Busy flag*/
#define FLASH_FLAG_BANK1_EOP                 FLASH_FLAG_EOP       /*!< FLASH BANK1 End of Operation flag */
#define FLASH_FLAG_BANK1_PGERR               FLASH_FLAG_PGERR     /*!< FLASH BANK1 Program error flag */
#define FLASH_FLAG_BANK1_WRPRTERR            FLASH_FLAG_WRPRTERR  /*!< FLASH BANK1 Write protected error flag */


#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32)0xFFFFFFCA) == 0x00000000) && ((FLAG) != 0x00000000))
#define IS_FLASH_GET_FLAG(FLAG)  (((FLAG) == FLASH_FLAG_BSY) || ((FLAG) == FLASH_FLAG_EOP) || \
                                  ((FLAG) == FLASH_FLAG_PGERR) || ((FLAG) == FLASH_FLAG_WRPRTERR) || \
								  ((FLAG) == FLASH_FLAG_BANK1_BSY) || ((FLAG) == FLASH_FLAG_BANK1_EOP) || \
                                  ((FLAG) == FLASH_FLAG_BANK1_PGERR) || ((FLAG) == FLASH_FLAG_BANK1_WRPRTERR) || \
                                  ((FLAG) == FLASH_FLAG_OPTERR))

#define IS_FLASH_WRPROT_PAGE(PAGE) (((PAGE) != 0x00000000))

#define IS_FLASH_ADDRESS(ADDRESS) (((ADDRESS) >= 0x08000000) && ((ADDRESS) < 0x080FFFFF))

#define IS_OB_DATA_ADDRESS(ADDRESS) (((ADDRESS) == 0x1FFFF804) || ((ADDRESS) == 0x1FFFF806))

/* FLASH Keys */
#define RDP_Key                  ((uint16)0x00A5)
#define FLASH_KEY1               ((uint32)0x45670123)
#define FLASH_KEY2               ((uint32)0xCDEF89AB)

/* Access control register */

#define FLASH_ACR_PRFTBS_BIT            5
#define FLASH_ACR_PRFTBE_BIT            4
#define FLASH_ACR_HLFCYA_BIT            3

#define FLASH_ACR_PRFTBS                BIT(FLASH_ACR_PRFTBS_BIT)
#define FLASH_ACR_PRFTBE                BIT(FLASH_ACR_PRFTBE_BIT)
#define FLASH_ACR_HLFCYA                BIT(FLASH_ACR_HLFCYA_BIT)
#define FLASH_ACR_LATENCY               0x7

/* Status register */

#define FLASH_SR_EOP_BIT                5
#define FLASH_SR_WRPRTERR_BIT           4
#define FLASH_SR_PGERR_BIT              2
#define FLASH_SR_BSY_BIT                0

#define FLASH_SR_EOP                    BIT(FLASH_SR_EOP_BIT)
#define FLASH_SR_WRPRTERR               BIT(FLASH_SR_WRPRTERR_BIT)
#define FLASH_SR_PGERR                  BIT(FLASH_SR_PGERR_BIT)
#define FLASH_SR_BSY                    BIT(FLASH_SR_BSY_BIT)

/* Control register */

#define FLASH_CR_EOPIE_BIT              12
#define FLASH_CR_ERRIE_BIT              10
#define FLASH_CR_OPTWRE_BIT             9
#define FLASH_CR_LOCK_BIT               7
#define FLASH_CR_STRT_BIT               6
#define FLASH_CR_OPTER_BIT              5
#define FLASH_CR_OPTPG_BIT              4
#define FLASH_CR_MER_BIT                2
#define FLASH_CR_PER_BIT                1
#define FLASH_CR_PG_BIT                 0

#define FLASH_CR_EOPIE                  BIT(FLASH_CR_EOPIE_BIT)
#define FLASH_CR_ERRIE                  BIT(FLASH_CR_ERRIE_BIT)
#define FLASH_CR_OPTWRE                 BIT(FLASH_CR_OPTWRE_BIT)
#define FLASH_CR_LOCK                   BIT(FLASH_CR_LOCK_BIT)
#define FLASH_CR_STRT                   BIT(FLASH_CR_STRT_BIT)
#define FLASH_CR_OPTER                  BIT(FLASH_CR_OPTER_BIT)
#define FLASH_CR_OPTPG                  BIT(FLASH_CR_OPTPG_BIT)
#define FLASH_CR_MER                    BIT(FLASH_CR_MER_BIT)
#define FLASH_CR_PER                    BIT(FLASH_CR_PER_BIT)
#define FLASH_CR_PG                     BIT(FLASH_CR_PG_BIT)


#define CR_PG_Reset              ((uint32)0x00001FFE)
#define CR_PER_Reset             ((uint32)0x00001FFD)
#define CR_MER_Reset             ((uint32)0x00001FFB)
#define CR_OPTPG_Reset           ((uint32)0x00001FEF)
#define CR_OPTER_Reset           ((uint32)0x00001FDF)



/* Option byte register */

#define FLASH_OBR_nRST_STDBY_BIT        4
#define FLASH_OBR_nRST_STOP_BIT         3
#define FLASH_OBR_WDG_SW_BIT            2
#define FLASH_OBR_RDPRT_BIT             1
#define FLASH_OBR_OPTERR_BIT            0

#define FLASH_OBR_DATA1                 (0xFF << 18)
#define FLASH_OBR_DATA0                 (0xFF << 10)
#define FLASH_OBR_USER                  0x3FF
#define FLASH_OBR_nRST_STDBY            BIT(FLASH_OBR_nRST_STDBY_BIT)
#define FLASH_OBR_nRST_STOP             BIT(FLASH_OBR_nRST_STOP_BIT)
#define FLASH_OBR_WDG_SW                BIT(FLASH_OBR_WDG_SW_BIT)
#define FLASH_OBR_RDPRT                 BIT(FLASH_OBR_RDPRT_BIT)
#define FLASH_OBR_OPTERR                BIT(FLASH_OBR_OPTERR_BIT)

/*
 * Setup routines
 */

#define FLASH_WAIT_STATE_0              0x0
#define FLASH_WAIT_STATE_1              0x1
#define FLASH_WAIT_STATE_2              0x2

void flash_enable_prefetch(void);
void flash_set_latency(uint32 wait_states);

/*
 * 2013-06-12 ROBOTIS ported from ST library for making EEPROM Emulation library
 * */
void FLASH_ClearFlag(uint32 FLASH_FLAG);
FLASH_Status FLASH_ProgramHalfWord(uint32 Address, uint16 Data);
FLASH_Status FLASH_ProgramWord(uint32 Address, uint32 Data);
FLASH_Status FLASH_ProgramOptionByteData(uint32 Address, uint8 Data);
FLASH_Status FLASH_WaitForLastOperation(uint32 Timeout);
FLASH_Status FLASH_GetBank1Status(void);
FLASH_Status FLASH_ErasePage(uint32 Page_Address);
FLASH_Status FLASH_EraseAllPages(void);
void FLASH_Unlock(void);
void FLASH_UnlockBank1(void);
void FLASH_Lock(void);
void FLASH_LockBank1(void);


#ifdef __cplusplus
}
#endif

#endif


