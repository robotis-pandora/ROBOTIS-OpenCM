#include "HaViMo2.h"

extern Dynamixel *pDxl;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HaViMo2_Controller::capture(void)
{
	pDxl->writeByte(HaViMo2_ID, 0, 0);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool HaViMo2_Controller::ready(void)
{
	// Ping HaViMo2
		// If responds => done processing last image, get results
		// Else => still processing, wait/try again later
	pDxl->ping(HaViMo2_ID);
	if (pDxl->getResult()==(1<<COMM_RXSUCCESS))
	{
		return true;
	}
	return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t HaViMo2_Controller::recover(void)
{
	hvm2rb.valid = 0;
	uint8_t iter;
	for (iter=0; iter<15; iter++)
	{
		hvm2rb.rb[iter].Index=0;
		hvm2rb.rb[iter].Color=0;
	}

	for (iter=0; iter<15; iter++)
	{
		pDxl->setTxPacketId(HaViMo2_ID);
		pDxl->setTxPacketInstruction(INST_READ);
		pDxl->setTxPacketParameter(0, ((iter+1)*16));
		pDxl->setTxPacketParameter(1, 16);
		pDxl->setTxPacketLength(2);

		pDxl->txrxPacket();

	// Dxl buffer is not cleared prior to RX
	//  If not checking for result, will grab from last valid RX packet
		if ( (pDxl->getResult()==(1<<COMM_RXSUCCESS)) &&
				(pDxl->getRxPacketLength()==(16+2)) )
		{
			if (pDxl->getRxPacketParameter(1)>0)
			{
				hvm2rb.valid++;

				hvm2rb.rb[hvm2rb.valid-1].Index=pDxl->getRxPacketParameter(0);
				hvm2rb.rb[hvm2rb.valid-1].Color=pDxl->getRxPacketParameter(1);
				hvm2rb.rb[hvm2rb.valid-1].NumPix=
					(
						(uint16_t)pDxl->getRxPacketParameter(2)+
						((uint16_t)pDxl->getRxPacketParameter(3)<<8)
					);
				hvm2rb.rb[hvm2rb.valid-1].SumX=
					(
						((uint32_t)pDxl->getRxPacketParameter(4)+
						((uint32_t)pDxl->getRxPacketParameter(5)<<8)+
						((uint32_t)pDxl->getRxPacketParameter(6)<<16))
					);
				hvm2rb.rb[hvm2rb.valid-1].SumY=
					(
						((uint32_t)pDxl->getRxPacketParameter(8)+
						((uint32_t)pDxl->getRxPacketParameter(9)<<8)+
						((uint32_t)pDxl->getRxPacketParameter(10)<<16))
					);
				hvm2rb.rb[hvm2rb.valid-1].MaxX=pDxl->getRxPacketParameter(12);
				hvm2rb.rb[hvm2rb.valid-1].MinX=pDxl->getRxPacketParameter(13);
				hvm2rb.rb[hvm2rb.valid-1].MaxY=pDxl->getRxPacketParameter(14);
				hvm2rb.rb[hvm2rb.valid-1].MinY=pDxl->getRxPacketParameter(15);
			}
		}
 	}
   return hvm2rb.valid;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t HaViMo2_Controller::regions(void)
{
	return hvm2rb.valid;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t HaViMo2_Controller::color(uint8_t region_index)
{
	if ( (region_index<16) && (hvm2rb.rb[region_index].Index>0) )
		return hvm2rb.rb[region_index].Color;
	else
		return 0xFF;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint16_t HaViMo2_Controller::size(uint8_t region_index)
{
	if ( (region_index<16) && (hvm2rb.rb[region_index].Index>0) )
		return hvm2rb.rb[region_index].NumPix;
	else
		return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t HaViMo2_Controller::avgX(uint8_t region_index)
{
	if ( (region_index<16) && (hvm2rb.rb[region_index].Index>0) )
		return (hvm2rb.rb[region_index].SumX / hvm2rb.rb[region_index].NumPix);
	else
		return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t HaViMo2_Controller::avgY(uint8_t region_index)
{
	if ( (region_index<16) && (hvm2rb.rb[region_index].Index>0) )
		return (hvm2rb.rb[region_index].SumY / hvm2rb.rb[region_index].NumPix);
	else
		return 0;
}

