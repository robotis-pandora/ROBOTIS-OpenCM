#include "HaViMo2.h"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HaViMo2_Controller::capture(void)
{
	Dxl.writeByte(HaViMo2_ID, 0, 0);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool HaViMo2_Controller::ready(void)
{
	// Ping HaViMo2
		// If responds => done processing last image, get results
		// Else => still processing, wait/try again later
	Dxl.ping(HaViMo2_ID);
	if (Dxl.getResult()==(1<<COMM_RXSUCCESS))
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
		Dxl.setTxPacketId(HaViMo2_ID);
		Dxl.setTxPacketInstruction(INST_READ);
		Dxl.setTxPacketParameter(0, ((iter+1)*16));
		Dxl.setTxPacketParameter(1, 16);
		Dxl.setTxPacketLength(2);

		Dxl.txrxPacket();

	// Dxl buffer is not cleared prior to RX
	//  If not checking for result, will grab from last valid RX packet
		if ( (Dxl.getResult()==(1<<COMM_RXSUCCESS)) &&
				(Dxl.getRxPacketLength()==(16+2)) )
		{
			if (Dxl.getRxPacketParameter(1)>0)
			{
				hvm2rb.valid++;

				hvm2rb.rb[hvm2rb.valid-1].Index=Dxl.getRxPacketParameter(0);
				hvm2rb.rb[hvm2rb.valid-1].Color=Dxl.getRxPacketParameter(1);
				hvm2rb.rb[hvm2rb.valid-1].NumPix=
					(
						(uint16_t)Dxl.getRxPacketParameter(2)+
						((uint16_t)Dxl.getRxPacketParameter(3)<<8)
					);
				hvm2rb.rb[hvm2rb.valid-1].SumX=
					(
						((uint32_t)Dxl.getRxPacketParameter(4)+
						((uint32_t)Dxl.getRxPacketParameter(5)<<8)+
						((uint32_t)Dxl.getRxPacketParameter(6)<<16))
					);
				hvm2rb.rb[hvm2rb.valid-1].SumY=
					(
						((uint32_t)Dxl.getRxPacketParameter(8)+
						((uint32_t)Dxl.getRxPacketParameter(9)<<8)+
						((uint32_t)Dxl.getRxPacketParameter(10)<<16))
					);
				hvm2rb.rb[hvm2rb.valid-1].MaxX=Dxl.getRxPacketParameter(12);
				hvm2rb.rb[hvm2rb.valid-1].MinX=Dxl.getRxPacketParameter(13);
				hvm2rb.rb[hvm2rb.valid-1].MaxY=Dxl.getRxPacketParameter(14);
				hvm2rb.rb[hvm2rb.valid-1].MinY=Dxl.getRxPacketParameter(15);
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

