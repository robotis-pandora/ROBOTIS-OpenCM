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
 * @file flash.c
 * @brief Flash management functions
 */

//#include "libpandora.h"
#include "flash.h"
#include "bitband.h"
#include "util.h"

/* Delay definition */
#define EraseTimeout          ((uint32)0x000B0000)
#define ProgramTimeout        ((uint32)0x00002000)

/**
 * @brief Turn on the hardware prefetcher.
 */
void flash_enable_prefetch(void) {
    *bb_perip(&FLASH_BASE->ACR, FLASH_ACR_PRFTBE_BIT) = 1;
}

/**
 * @brief Set flash wait states
 *
 * See ST PM0042, section 3.1 for restrictions on the acceptable value
 * of wait_states for a given SYSCLK configuration.
 *
 * @param wait_states number of wait states (one of
 *                    FLASH_WAIT_STATE_0, FLASH_WAIT_STATE_1,
 *                    FLASH_WAIT_STATE_2).
 */
void flash_set_latency(uint32 wait_states) {
    uint32 val = FLASH_BASE->ACR;

    val &= ~FLASH_ACR_LATENCY;
    val |= wait_states;

    FLASH_BASE->ACR = val;
}


/**
  * @brief  Clears the FLASH's pending flags.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices, this function clears Bank1 or Bank2’s pending flags
  *         - For other devices, it clears Bank1’s pending flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_FLAG_PGERR: FLASH Program error flag
  *     @arg FLASH_FLAG_WRPRTERR: FLASH Write protected error flag
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  * @retval None
  */
void FLASH_ClearFlag(uint32 FLASH_FLAG)
{

  /* Check the parameters */
  ASSERT(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;

  /* Clear the flags */
  FLASH_BASE->SR = FLASH_FLAG;

}
/**
  * @brief  Unlocks the FLASH Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function unlocks Bank1 and Bank2.
  *         - For all other devices it unlocks Bank1 and it is equivalent
  *           to FLASH_UnlockBank1 function..
  * @param  None
  * @retval None
  */
void FLASH_Unlock(void)
{
  /* Authorize the FPEC of Bank1 Access */
  FLASH_BASE->KEYR = FLASH_KEY1;
  FLASH_BASE->KEYR = FLASH_KEY2;

#ifdef STM32F10X_XL
  /* Authorize the FPEC of Bank2 Access */
  FLASH_BASE->KEYR2 = FLASH_KEY1;
  FLASH_BASE->KEYR2 = FLASH_KEY2;
#endif /* STM32F10X_XL */
}
/**
  * @brief  Unlocks the FLASH Bank1 Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function unlocks Bank1.
  *         - For all other devices it unlocks Bank1 and it is
  *           equivalent to FLASH_Unlock function.
  * @param  None
  * @retval None
  */
void FLASH_UnlockBank1(void)
{
  /* Authorize the FPEC of Bank1 Access */
  FLASH_BASE->KEYR = FLASH_KEY1;
  FLASH_BASE->KEYR = FLASH_KEY2;
}


/**
  * @brief  Locks the FLASH Program Erase Controller.
  * @note   This function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function Locks Bank1 and Bank2.
  *         - For all other devices it Locks Bank1 and it is equivalent
  *           to FLASH_LockBank1 function.
  * @param  None
  * @retval None
  */
void FLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank1 */
  FLASH_BASE->CR |= FLASH_CR_LOCK;

#ifdef STM32F10X_XL
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank2 */
  FLASH_BASE->CR2 |= FLASH_CR_LOCK;
#endif /* STM32F10X_XL */
}

/**
  * @brief  Locks the FLASH Bank1 Program Erase Controller.
  * @note   this function can be used for all STM32F10x devices.
  *         - For STM32F10X_XL devices this function Locks Bank1.
  *         - For all other devices it Locks Bank1 and it is equivalent
  *           to FLASH_Lock function.
  * @param  None
  * @retval None
  */
void FLASH_LockBank1(void)
{
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank1 */
  FLASH_BASE->CR |= FLASH_CR_LOCK;//CR_LOCK_Set;
}
/**
  * @brief  Returns the FLASH Bank1 Status.
  * @note   This function can be used for all STM32F10x devices, it is equivalent
  *         to FLASH_GetStatus function.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP or FLASH_COMPLETE
  */
FLASH_Status FLASH_GetBank1Status(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;

  if((FLASH_BASE->SR & FLASH_FLAG_BANK1_BSY) == FLASH_FLAG_BSY)
  {
    flashstatus = FLASH_BUSY;
  }
  else
  {
    if((FLASH_BASE->SR & FLASH_FLAG_BANK1_PGERR) != 0)
    {
      flashstatus = FLASH_ERROR_PG;
    }
    else
    {
      if((FLASH_BASE->SR & FLASH_FLAG_BANK1_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status */
  return flashstatus;
}
/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @note   This function can be used for all STM32F10x devices,
  *         it is equivalent to FLASH_WaitForLastBank1Operation.
  *         - For STM32F10X_XL devices this function waits for a Bank1 Flash operation
  *           to complete or a TIMEOUT to occur.
  *         - For all other devices it waits for a Flash operation to complete
  *           or a TIMEOUT to occur.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32 Timeout)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check for the Flash Status */
  status = FLASH_GetBank1Status();
  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetBank1Status();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}

/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  *   This parameter can be 0x1FFFF804 or 0x1FFFF806.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ProgramOptionByteData(uint32 Address, uint8 Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  ASSERT(IS_OB_DATA_ADDRESS(Address));
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* Authorize the small information block programming */
	  FLASH_BASE->OPTKEYR = FLASH_KEY1;
	  FLASH_BASE->OPTKEYR = FLASH_KEY2;
    /* Enables the Option Bytes Programming operation */
	  FLASH_BASE->CR |= FLASH_CR_OPTPG;//CR_OPTPG_Set;
    *(__io uint16*)Address = Data;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
    	FLASH_BASE->CR &= CR_OPTPG_Reset;
    }
  }
  /* Return the Option Byte Data Program Status */
  return status;
}

/**
  * @brief  Programs a half word at a specified address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ProgramHalfWord(uint32 Address, uint16 Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  ASSERT(IS_FLASH_ADDRESS(Address));


  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new data */
	  FLASH_BASE->CR |= FLASH_CR_PG;//CR_PG_Set;

    *(__io uint16*)Address = Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    /* Disable the PG Bit */
    FLASH_BASE->CR &= CR_PG_Reset;
  }
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a word at a specified address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ProgramWord(uint32 Address, uint32 Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  __io uint32 tmp = 0;

  /* Check the parameters */
  ASSERT(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new first
    half word */
    FLASH_BASE->CR |= FLASH_CR_PG;//CR_PG_Set;

    *(__io uint16*)Address = (uint16)Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new second
      half word */
      tmp = Address + 2;

      *(__io uint16*) tmp = Data >> 16;

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);

      /* Disable the PG Bit */
      FLASH_BASE->CR &= CR_PG_Reset;
    }
    else
    {
      /* Disable the PG Bit */
      FLASH_BASE->CR &= CR_PG_Reset;
    }
  }

  /* Return the Program Status */
  return status;
}

/**
  * @brief  Erases a specified FLASH page.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32 Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  ASSERT(IS_FLASH_ADDRESS(Page_Address));


  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase the page */
    FLASH_BASE->CR|= FLASH_CR_PER;//CR_PER_Set;
    FLASH_BASE->AR = Page_Address;
    FLASH_BASE->CR|= FLASH_CR_STRT;//CR_STRT_Set;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    /* Disable the PER Bit */
    FLASH_BASE->CR &= CR_PER_Reset;
  }
  /* Return the Erase Status */
  return status;
}

/**
  * @brief  Erases all FLASH pages.
  * @note   This function can be used for all STM32F10x devices.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages */
	  FLASH_BASE->CR |= FLASH_CR_MER;//CR_MER_Set;
	  FLASH_BASE->CR |= FLASH_CR_STRT;//CR_STRT_Set;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    /* Disable the MER Bit */
    FLASH_BASE->CR &= CR_MER_Reset;
  }

  /* Return the Erase Status */
  return status;
}
