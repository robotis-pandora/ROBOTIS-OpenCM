/******************* (C) COPYRIGHT 2011 STMicroelectronics **************************/

/* Includes ------------------------------------------------------------------*/
#include "eeprom_core.h"

u8 gbUsePage;
long glWritePtr;

u16 EEPROM_Read( u16 wVirtualAddrss )
{

	long lSrcPtr,lBasePtr;
	long lTempVirtualAddrss;

	if( gbUsePage == PAGE0 )	lBasePtr = PAGE0_BASE_ADDRESS+4;
	else						lBasePtr = PAGE1_BASE_ADDRESS+4;

	lSrcPtr = glWritePtr;

	while(1)
	{
		if( lBasePtr > lSrcPtr )					return 0xFFFF;

		lTempVirtualAddrss = *((u16 * ) lSrcPtr);

		if( wVirtualAddrss == lTempVirtualAddrss )	return (*((u16 * ) (lSrcPtr+2)));

		lSrcPtr-=4;
	}
}

void EEPROM_Write( u16 wVirtualAddrss , u16 wData )
{
	u16 wTag0,wTag1;

	wTag0 = *(u16 *)(PAGE0_BASE_ADDRESS);
	wTag1 = *(u16 *)(PAGE1_BASE_ADDRESS);

	// eeprom 영역이 가득 찼는지 검사해서, 가득찼다면, eeprom page를 변환해준다.
	if( (gbUsePage == PAGE0) && ( glWritePtr > (PAGE0_BASE_ADDRESS + FLASH_PAGE_SIZE - 4) ) )
	{
		FLASH_ProgramHalfWord( PAGE0_BASE_ADDRESS, STATUS_EEPROM_FULL );
		gbUsePage = PAGE1;
		if( wTag1 == STATUS_EEPROM_USE )	EEPROM_ERASE(PAGE1);
		FLASH_ProgramHalfWord( PAGE1_BASE_ADDRESS, STATUS_EEPROM_USE );
		EEPROM_COPY_PAGE(PAGE1,PAGE0); // copy from 0 to 1.
		EEPROM_ERASE(PAGE0);
		EEPROM_WRITE_PONTER_SETUP(gbUsePage);
	}
	else if( (gbUsePage == PAGE1) && ( glWritePtr > (PAGE1_BASE_ADDRESS + FLASH_PAGE_SIZE - 4) ) )
	{
		FLASH_ProgramHalfWord( PAGE1_BASE_ADDRESS, STATUS_EEPROM_FULL );
		gbUsePage = PAGE0;
		if( wTag0 == STATUS_EEPROM_USE )	EEPROM_ERASE(PAGE0);
		FLASH_ProgramHalfWord( PAGE0_BASE_ADDRESS, STATUS_EEPROM_USE );
		EEPROM_COPY_PAGE(PAGE0,PAGE1); // copy from 1 to 0.
		EEPROM_ERASE(PAGE1);
		EEPROM_WRITE_PONTER_SETUP(gbUsePage);
	}


//	TxDLong16(glWritePtr); TxDString(" x "); TxDLong16(wVirtualAddrss); TxDString("\r\n");
	FLASH_ProgramHalfWord( glWritePtr, wVirtualAddrss );
	FLASH_ProgramHalfWord( glWritePtr+2, wData );
	glWritePtr +=4;
}


void EEPROM_INIT(void)
{
	u16 wTag0,wTag1;

	wTag0 = *(u16 *)(PAGE0_BASE_ADDRESS);
	wTag1 = *(u16 *)(PAGE1_BASE_ADDRESS);

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR |
			FLASH_FLAG_WRPRTERR | FLASH_FLAG_OPTERR);
//	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
//			FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

	while(1)
	{
		if( wTag0 == STATUS_EEPROM_EMPTY )
		{
			if( wTag1 == STATUS_EEPROM_EMPTY )
			{
				FLASH_ProgramHalfWord( PAGE0_BASE_ADDRESS, STATUS_EEPROM_USE );
				gbUsePage = PAGE0;
			}
			else if( wTag1 == STATUS_EEPROM_USE )
			{
				gbUsePage = PAGE1;
			}
			else if( wTag1 == STATUS_EEPROM_FULL )
			{
				gbUsePage = PAGE1;
				//Erase_EEPROM(PAGE0);
				FLASH_ProgramHalfWord( PAGE0_BASE_ADDRESS, STATUS_EEPROM_USE );
				EEPROM_COPY_PAGE(PAGE0,PAGE1); // copy from 1 to 0.
				EEPROM_ERASE(PAGE1);
			}
		}
		else if( wTag0 == STATUS_EEPROM_USE )
		{
			if( wTag1 == STATUS_EEPROM_EMPTY )
			{
				gbUsePage = PAGE0;
			}
			else if( wTag1 == STATUS_EEPROM_USE )
			{
				// impossible.
							//while(1); // print error code.
			}
			else if( wTag1 == STATUS_EEPROM_FULL )
			{ //copy 되던 도중, 실패한 경우 이므로, page 0를 삭제하고, 다시 카피한다.
				gbUsePage = PAGE1;
				EEPROM_ERASE(PAGE0);
				FLASH_ProgramHalfWord( PAGE0_BASE_ADDRESS, STATUS_EEPROM_USE );
				EEPROM_COPY_PAGE(PAGE0,PAGE1); // copy from 1 to 0.
				EEPROM_ERASE(PAGE1);
			}
		}
		else if( wTag0 == STATUS_EEPROM_FULL )
		{
			if( wTag1 == STATUS_EEPROM_EMPTY )
			{
				gbUsePage = PAGE1;
				//Erase_EEPROM(1);
				FLASH_ProgramHalfWord( PAGE1_BASE_ADDRESS, STATUS_EEPROM_USE );
				EEPROM_COPY_PAGE(PAGE1,PAGE0); // copy from 0 to 1.
				EEPROM_ERASE(PAGE0);
			}
			else if( wTag1 == STATUS_EEPROM_USE )
			{
				gbUsePage = PAGE1;
				EEPROM_ERASE(PAGE1);
				FLASH_ProgramHalfWord( PAGE1_BASE_ADDRESS, STATUS_EEPROM_USE );
				EEPROM_COPY_PAGE(PAGE1,PAGE0); // copy from 0 to 1.
				EEPROM_ERASE(PAGE0);
			}
			else if( wTag1 == STATUS_EEPROM_FULL )
			{
				// impossible.
				//while(1); // print error code.
			}
		}

		EEPROM_WRITE_PONTER_SETUP(gbUsePage);

		if( glWritePtr == PAGE0_BASE_ADDRESS + FLASH_PAGE_SIZE - 4)
			FLASH_ProgramHalfWord( PAGE0_BASE_ADDRESS, STATUS_EEPROM_FULL );
		else if( glWritePtr == PAGE1_BASE_ADDRESS + FLASH_PAGE_SIZE - 4)
			FLASH_ProgramHalfWord( PAGE1_BASE_ADDRESS, STATUS_EEPROM_FULL );
		else
			break;
	}


}

void EEPROM_WRITE_PONTER_SETUP(u8 Page)
{
	long lTempPointer, lBaseAddress;
	u16 wTempData;


	if( Page == PAGE0 )			lBaseAddress = PAGE0_BASE_ADDRESS;
	else if( Page == PAGE1 )	lBaseAddress = PAGE1_BASE_ADDRESS;

	lTempPointer = lBaseAddress + FLASH_PAGE_SIZE - 4;

	while(1)
	{
		wTempData = *((u16 * ) lTempPointer);
		if( wTempData != 0xFFFF )
		{
			glWritePtr = lTempPointer+4;
			return ;
		}
		lTempPointer -=4;

		if( lTempPointer == lBaseAddress )
		{
			glWritePtr = lBaseAddress + 4;
			return ;
		}
	}

}

void EEPROM_COPY_PAGE(u8 DestPage, u8 SrcPage)
{
	long lVirtualAddress = MAX_VIRTUAL_ADDRESS;
	long lTempVirtualAddrss;
	long lSrcPtr, lDestPtr,lBasePtr;

	if( SrcPage == PAGE0 )
	{
		lBasePtr = PAGE0_BASE_ADDRESS+4;
		lDestPtr = PAGE1_BASE_ADDRESS+4;
	}
	else
	{
		lBasePtr = PAGE1_BASE_ADDRESS+4;
		lDestPtr = PAGE0_BASE_ADDRESS+4;
	}


	while(1)
	{
		if( lVirtualAddress < 0 ) break;
		lSrcPtr = glWritePtr;

		while(1)
		{
			if( lBasePtr > lSrcPtr )
			{
				FLASH_ProgramWord ( lDestPtr + (lVirtualAddress*4) , (lVirtualAddress | 0xFFFF0000) );
				break;
			}

			lTempVirtualAddrss = *((u16 * ) lSrcPtr);
			if( lVirtualAddress == lTempVirtualAddrss )
			{
				FLASH_ProgramWord ( lDestPtr + (lVirtualAddress*4) , *((long * ) lSrcPtr) );
				break;
			}
			lSrcPtr-=4;
		}

		lVirtualAddress--;
	}
}

void EEPROM_ERASE(u8 Page)
{
	FLASH_ClearFlag(/*FLASH_FLAG_BSY | FLASH_FLAG_EOP |*/ FLASH_FLAG_PGERR /*| FLASH_FLAG_WRPRTERR | FLASH_FLAG_OPTERR*/); // jason added 20130424

	FLASH_Status status = FLASH_COMPLETE;
	if(Page == PAGE0)
		status = FLASH_ErasePage(PAGE0_BASE_ADDRESS);
		//FLASH_EraseSector(GetSector(PAGE0_BASE_ADDRESS), VoltageRange_3);
	else
		status = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
		//FLASH_EraseSector(GetSector(PAGE1_BASE_ADDRESS), VoltageRange_3);

	if(status != FLASH_COMPLETE){
		return;
		//TxDString("erase fail!!\r\n");
	}
}

/* Private typedef --------------

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
