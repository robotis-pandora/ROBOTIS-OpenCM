/*
 * DynamixelPro.cpp
 *
 *  Created on: 2013. 4. 25.
 *      Author: ROBOTIS,.LTD.
 */

#include "DynamixelPro.h"
#include "dxl_pro.h"
#include "Arduino-compatibles.h"


extern usart_dev *gDynamixelUsartDev;

DynamixelPro::DynamixelPro(int devNum) {
	// TODO Auto-generated constructor stub

	if(devNum == 1){ //USART1 case

		gDynamixelUsartDev = USART1;


		///gbIsDynmixelUsed = &gbIsDynmixelUsart1Used;

	}else if(devNum == 2){//USART2 case
		/*
		 * [ROBOTIS] USART2 device should not be used in dynamixel bus
		 * */

		//gDynamixelUsartDev = USART2;  //forbidden
	}else if(devNum ==3 ){//USART3 case

		gDynamixelUsartDev = USART3;



	}else{
		gDynamixelUsartDev = USART1; // default is USART1
	}

}

DynamixelPro::~DynamixelPro() {
	// TODO Auto-generated destructor stub
}


void DynamixelPro::begin(int baud) {
	int i=0;
	uint32 Baudrate = 0;
	//TxDString("[DXL]start begin\r\n");


	if(gDynamixelUsartDev == USART1){
		afio_remap(AFIO_REMAP_USART1);
	}

#ifdef BOARD_CM900  //Engineering version case

	 gpio_set_mode(PORT_ENABLE_TXD, PIN_ENABLE_TXD, GPIO_OUTPUT_PP);
	 gpio_set_mode(PORT_ENABLE_RXD, PIN_ENABLE_RXD, GPIO_OUTPUT_PP);
	 gpio_write_bit(PORT_ENABLE_TXD, PIN_ENABLE_TXD, 0 );// TX Disable
	 gpio_write_bit(PORT_ENABLE_RXD, PIN_ENABLE_RXD, 1 );// RX Enable
#else

	 if(gDynamixelUsartDev == USART1){
			 // initialize GPIO D20(PB6), D21(PB7) as DXL TX, RX respectively
		 gpio_set_mode(PORT_DXL_TXD, PIN_DXL_TXD, GPIO_AF_OUTPUT_PP);
		 gpio_set_mode(PORT_DXL_RXD, PIN_DXL_RXD, GPIO_INPUT_FLOATING);
		 gpio_set_mode(PORT_TXRX_DIRECTION, PIN_TXRX_DIRECTION, GPIO_OUTPUT_PP);
		 gpio_write_bit(PORT_TXRX_DIRECTION, PIN_TXRX_DIRECTION, 0 );// RX Enable

	 }else if(gDynamixelUsartDev == USART2){
		 /*
		  *[ROBOTIS] USART2 device should not be used in dynamixel bus
		  * */
	 }else if(gDynamixelUsartDev == USART3){
		 // initialize GPIO D20(PB6), D21(PB7) as DXL TX, RX respectively
		 gpio_set_mode(PORT_DXL_TXD3, PIN_DXL_TXD3, GPIO_AF_OUTPUT_PP);
		 gpio_set_mode(PORT_DXL_RXD3, PIN_DXL_RXD3, GPIO_INPUT_FLOATING);
		 gpio_set_mode(PORT_TXRX_DIRECTION3, PIN_TXRX_DIRECTION3, GPIO_OUTPUT_PP);
		 gpio_write_bit(PORT_TXRX_DIRECTION3, PIN_TXRX_DIRECTION3, 0 );// RX Enable

	 }
#endif


	 //Initialize USARTx device
	 usart_init(gDynamixelUsartDev);

	 //Calculate baudrate, refer to ROBOTIS support page.
	 Baudrate = dxl_get_baudrate(baud);

	 if(gDynamixelUsartDev == USART1){
		 usart_set_baud_rate(gDynamixelUsartDev, STM32_PCLK2, Baudrate);
	 }else{
		 usart_set_baud_rate(gDynamixelUsartDev, STM32_PCLK1, Baudrate);
	 }

	nvic_irq_set_priority(gDynamixelUsartDev->irq_num, 0);//[ROBOTIS][ADD] 2013-04-10 set to priority 0
	usart_attach_interrupt(gDynamixelUsartDev, dxlProInterrupt);
	usart_enable(gDynamixelUsartDev);

	delay(80);

	gbRxLengthEx = 0;
	clearBuffer256Ex();

	mResult= 0;
	for(i=0; i<32; i++){
		mBulkData[i].iID = 0;
		mBulkData[i].iAddr = 0; //get address
		mBulkData[i].iLength = 0;
		mBulkData[i].iError = 0; //Error code
		mBulkData[i].iData[0] = 0; //DATA1
		mBulkData[i].iData[1] = 0; //DATA2
		mBulkData[i].iData[2] = 0; //DATA3
		mBulkData[i].iData[3] = 0; //DATA4
	}
}


void DynamixelPro::end(void){

}
/*
 *  [ROBOTIS][ADD][START] 2013-04-09 support read and write on dxl bus
 * */

byte DynamixelPro::readRaw(void){

	return RxByteFromDXLEx();
}
void DynamixelPro::writeRaw(uint8 value){

	if(gDynamixelUsartDev == USART1){
		DXL_TXD
	}else{
		DXL_TXD3
	}
	TxByteToDXLEx(value);

	if(gDynamixelUsartDev == USART1){
		DXL_RXD
	}else{
		DXL_RXD3
	}


}
/*
 * @brief : if data coming from dxl bus, returns 1, or if not, returns 0.
 *
 */
byte DynamixelPro::available(void){

	return checkNewArriveEx();
}

byte DynamixelPro::readByte(byte bID, int wAddress){
	mResult = 0;
	gbpParameterEx[0]	= (unsigned char)DXL_LOBYTE(wAddress);
	gbpParameterEx[1]	= (unsigned char)DXL_HIBYTE(wAddress);
	gbpParameterEx[2]	= 1; //1byte
	gbpParameterEx[3]	= 0;
	if(txrx_PacketEx(bID, INST_READ_EX, 4)){
		mResult = 1;
		return(gbpRxBufferEx[9]);
	}else{
		mResult = 0;
		return 0xff;
	}


}
byte DynamixelPro::writeByte(uint8 bID, int wAddress, byte bData){
	mResult = 0;
	//insert wAddress to parameter bucket
	gbpParameterEx[0]	= (unsigned char)DXL_LOBYTE(wAddress);
	gbpParameterEx[1]	= (unsigned char)DXL_HIBYTE(wAddress);
	//insert data to parameter bucket
	gbpParameterEx[2]	= bData;
	mResult = txrx_PacketEx(bID, INST_WRITE_EX, 3); //// parameter length 4 = 2(address)+2(data)
	clearBuffer256Ex();
	return mResult;

}
word DynamixelPro::readWord(byte bID, int wAddress){

	mResult = 0;
	gbpParameterEx[0]	= (unsigned char)DXL_LOBYTE(wAddress);
	gbpParameterEx[1]	= (unsigned char)DXL_HIBYTE(wAddress);
	gbpParameterEx[2]	= 2; //2byte
	gbpParameterEx[3]	= 0;
	if(txrx_PacketEx(bID, INST_READ_EX, 4)){
		mResult = 1;
		return(DXL_MAKEWORD(gbpRxBufferEx[9], gbpRxBufferEx[10]));
	}else{
		mResult = 0;
		return 0xffff;
	}

}
byte DynamixelPro::writeWord(byte bID, int wAddress, word wData){
	mResult = 0;
	//insert wAddress to parameter bucket
	gbpParameterEx[0]	= (unsigned char)DXL_LOBYTE(wAddress);
	gbpParameterEx[1]	= (unsigned char)DXL_HIBYTE(wAddress);
	//insert data to parameter bucket
	gbpParameterEx[2]	= DXL_LOBYTE(wData);
	gbpParameterEx[3]	= DXL_HIBYTE(wData);

	mResult = txrx_PacketEx(bID, INST_WRITE_EX, 4); //// parameter length 4 = 2(address)+2(data)
	clearBuffer256Ex();

	return mResult;
}

byte DynamixelPro::writeDword( byte bID, int wAddress, unsigned int value ){
	mResult = 0;
	//insert wAddress to parameter bucket
	gbpParameterEx[0]	= (unsigned char)DXL_LOBYTE(wAddress);
	gbpParameterEx[1]	= (unsigned char)DXL_HIBYTE(wAddress);
	//insert data to parameter bucket
	gbpParameterEx[2]	= DXL_LOBYTE(DXL_LOWORD(value));
	gbpParameterEx[3]	= DXL_HIBYTE(DXL_LOWORD(value));
	gbpParameterEx[4]	= DXL_LOBYTE(DXL_HIWORD(value));
	gbpParameterEx[5]	= DXL_HIBYTE(DXL_HIWORD(value));

	mResult = txrx_PacketEx(bID, INST_WRITE_EX, 6); //// parameter length 4 = 2(address)+2(data)
	clearBuffer256Ex();
	return mResult;
}
uint32 DynamixelPro::readDword( byte bID, int wAddress ){
	mResult = 0;
	gbpParameterEx[0]	= (unsigned char)DXL_LOBYTE(wAddress);
	gbpParameterEx[1]	= (unsigned char)DXL_HIBYTE(wAddress);
	gbpParameterEx[2]	= 4; //4byte
	gbpParameterEx[3]	= 0;
	if(txrx_PacketEx(bID, INST_READ_EX, 4)){
		mResult = 1;
		return DXL_MAKEDWORD( DXL_MAKEWORD( gbpRxBufferEx[9], gbpRxBufferEx[10]),
							  DXL_MAKEWORD( gbpRxBufferEx[11], gbpRxBufferEx[12])
							 );
	}else{
		mResult = 0;
		return 0xffffffff;
	}
}



byte DynamixelPro::syncWrite(int start_addr, int data_length, byte *param, int param_length)
{
	mResult = 0;
	int i=0;
    gbpParameterEx[0]     = (unsigned char)DXL_LOBYTE(start_addr);
    gbpParameterEx[1]   = (unsigned char)DXL_HIBYTE(start_addr);
    gbpParameterEx[2]   = (unsigned char)DXL_LOBYTE(data_length);
    gbpParameterEx[3]   = (unsigned char)DXL_HIBYTE(data_length);
    for(i=0; i < param_length; i++){
    	gbpParameterEx[4+i] = param[i];
    }
    mResult = txrx_PacketEx(BROADCAST_ID, INST_SYNC_WRITE_EX, 4+param_length);
	return mResult;
}
byte DynamixelPro::syncWrite(int start_addr, int data_length, word *param, int param_length){
	mResult = 0;

	int i=0, j=0, k=0, num=0;
	num = param_length / (data_length + 1);

    gbpParameterEx[0]   = DXL_LOBYTE(start_addr);
    gbpParameterEx[1]   = DXL_HIBYTE(start_addr);
    gbpParameterEx[2]   = DXL_LOBYTE(data_length*4);
    gbpParameterEx[3]   = DXL_HIBYTE(data_length*4);

    for(i=4; i < (4+num*(1+data_length*2)); i+=(1+data_length*2) ){
    	gbpParameterEx[i]   = (byte)param[k++]; //ID
    	for(j=0; j < (data_length*2); j+=2){
    		gbpParameterEx[i+j+1] = DXL_LOBYTE(param[k]); //low byte
    		gbpParameterEx[i+j+2] = DXL_HIBYTE(param[k]); //high byte
			k++;
    	}

    }
    mResult = txrx_PacketEx(BROADCAST_ID, INST_SYNC_WRITE_EX, 4+param_length*2);
	return mResult;
}
byte DynamixelPro::syncWrite(int start_addr, int data_length, int *param, int param_length){
	mResult = 0;
	int i=0, j=0, k=0, num=0;
	num = param_length / (data_length + 1);

    gbpParameterEx[0]   = DXL_LOBYTE(start_addr);
    gbpParameterEx[1]   = DXL_HIBYTE(start_addr);
    gbpParameterEx[2]   = DXL_LOBYTE(data_length*4);
    gbpParameterEx[3]   = DXL_HIBYTE(data_length*4);

    for(i=4; i < (4+num*(1+data_length*4)); i+=(1+data_length*4) ){
    	gbpParameterEx[i]   = (byte)param[k++]; //ID
    	for(j=0; j < (data_length*4); j+=4){
    		gbpParameterEx[i+j+1] = DXL_LOBYTE(DXL_LOWORD(param[k])); //data
			gbpParameterEx[i+j+2] = DXL_HIBYTE(DXL_LOWORD(param[k]));
			gbpParameterEx[i+j+3] = DXL_LOBYTE(DXL_HIWORD(param[k]));
			gbpParameterEx[i+j+4] = DXL_HIBYTE(DXL_HIWORD(param[k]));
			k++;
    	}

    }
    mResult = txrx_PacketEx(BROADCAST_ID, INST_SYNC_WRITE_EX, 4+i);
	return mResult;
}
int DynamixelPro::bulkRead(byte *param, int param_length){
	mResult = 0;
	uint32 bulkReadlength=0;

	int n, i, k=0;
	int num = param_length / 5; // each length : 5 (ID ADDR_L ADDR_H LEN_L LEN_H)
	// int pkt_length = param_length + 3;  // 3 : INST CHKSUM_L CHKSUM_H
	//   unsigned char txpacket[MAXNUM_TXPACKET] = {0};
	//   unsigned char rxpacket[MAXNUM_RXPACKET] = {0};

	for(n=0; n < param_length; n++){
	gbpParameterEx[n] = param[n];

	}

	/************ TxRxPacket *************/
	// Wait for Bus Idle
	/*    while(comm->iBusUsing == 1)
	{
		//Sleep(0);
	}*/

	//mResult = txrx_PacketEx(BROADCAST_ID, INST_BULK_READ_EX, param_length);;
	mResult = tx_PacketEx(BROADCAST_ID, INST_BULK_READ_EX, param_length);

	if(mResult == 0)
		return 0;

	for(n = 0; n < num; n++){
	   // int id = param[n*5+0];

		bulkReadlength = rx_PacketEx(param_length+11);
		/*result =  DXL_MAKEDWORD(	DXL_MAKEWORD(gbpRxBufferEx[9],gbpRxBufferEx[10]),
						DXL_MAKEWORD(gbpRxBufferEx[11],gbpRxBufferEx[12])
					  );*/
		if(gbpRxBufferEx[PKT_INSTRUCTION] == 0x55){
			mBulkData[n].iID = gbpRxBufferEx[PKT_ID];
			mBulkData[n].iAddr = DXL_MAKEWORD(gbpParameterEx[5*n+1],gbpParameterEx[5*n+2]); //get address
			mBulkData[n].iLength = DXL_MAKEWORD(gbpParameterEx[5*n+3],gbpParameterEx[5*n+4]);//DXL_MAKEWORD(gbpRxBufferEx[PKT_LENGTH_L],gbpRxBufferEx[PKT_LENGTH_H]);
			//TxDStringC("iLength = ");TxDHex8C(mBulkData[n].iLength);TxDStringC("\r\n");
			mBulkData[n].iError = gbpRxBufferEx[PKT_INSTRUCTION+1]; //Error code
			for(i=0; i < mBulkData[n].iLength ; i++){
				mBulkData[n].iData[i] = gbpRxBufferEx[PKT_INSTRUCTION+2+i]; //DATA1
			}

		}
		for(k=0;k < MAXNUM_RXPACKET ; k++){
			gbpRxBufferEx[k] = 0; //buffer clear
		}
		clearBuffer256Ex();

	}
	return bulkReadlength;

}
int DynamixelPro::bulkRead(word *param, int param_length){
	mResult = 0;
	uint32 bulkReadlength=0;

	int n=0,k=0, i=0;
	int num = param_length / 3; // each length : 5 (ID ADDR_L ADDR_H LEN_L LEN_H)
	// int pkt_length = param_length + 3;  // 3 : INST CHKSUM_L CHKSUM_H
	//   unsigned char txpacket[MAXNUM_TXPACKET] = {0};
	//   unsigned char rxpacket[MAXNUM_RXPACKET] = {0};
	for(i=0; i < num*5 ; i+=5){
		gbpParameterEx[i]   = (byte)param[k++]; //ID
		for(n=0; n < 4;n+=2){
			gbpParameterEx[i+n+1] = DXL_LOBYTE(param[k]);
			gbpParameterEx[i+n+2] = DXL_HIBYTE(param[k]);
			k++;
		}

	}
	param_length = i;

	/************ TxRxPacket *************/
	// Wait for Bus Idle
	/*    while(comm->iBusUsing == 1)
	{
		//Sleep(0);
	}*/

	//mResult = txrx_PacketEx(BROADCAST_ID, INST_BULK_READ_EX, param_length);;
	mResult = tx_PacketEx(BROADCAST_ID, INST_BULK_READ_EX, param_length);

	if(mResult == 0)
		return 0;
	//TxDStringC("num = ");TxDHex8C(num);TxDStringC("\r\n");
	for(n = 0; n < num; n++){
	   // int id = param[n*5+0];

		bulkReadlength = rx_PacketEx(param_length+11);
		/*result =  DXL_MAKEDWORD(	DXL_MAKEWORD(gbpRxBufferEx[9],gbpRxBufferEx[10]),
						DXL_MAKEWORD(gbpRxBufferEx[11],gbpRxBufferEx[12])
					  );*/
		if(gbpRxBufferEx[PKT_INSTRUCTION] == 0x55){
			mBulkData[n].iID = gbpRxBufferEx[PKT_ID];
			mBulkData[n].iAddr = DXL_MAKEWORD(gbpParameterEx[5*n+1],gbpParameterEx[5*n+2]); //get address
			mBulkData[n].iLength = DXL_MAKEWORD(gbpParameterEx[5*n+3],gbpParameterEx[5*n+4]);//DXL_MAKEWORD(gbpRxBufferEx[PKT_LENGTH_L],gbpRxBufferEx[PKT_LENGTH_H]);
			//TxDStringC("iLength = ");TxDHex8C(mBulkData[n].iLength);TxDStringC("\r\n");
			mBulkData[n].iError = gbpRxBufferEx[PKT_INSTRUCTION+1]; //Error code
			for(i=0; i < mBulkData[n].iLength ; i++){
				mBulkData[n].iData[i] = gbpRxBufferEx[PKT_INSTRUCTION+2+i]; //DATA1
			}

		}
		for(k=0;k < MAXNUM_RXPACKET ; k++){
			gbpRxBufferEx[k] = 0; //buffer clear
		}
		clearBuffer256Ex();
		//TxDStringC("before\r\n");
		//while(!checkNewArriveEx());
		//TxDStringC("after\r\n");

	}
#ifdef CM9_DEBUG
	/*	TxDStringC("\r\n ->[TX Buffer]: ");
		PrintBufferEx(gbpTxBufferEx,(byte)mResult);
		TxDStringC("\r\n <-[RX Buffer]: ");
		PrintBufferEx(gbpRxBufferEx,(byte)bulkReadlength);*/
#endif
	return bulkReadlength;

}
byte DynamixelPro::getBulkByte(int id, int addr){
	int i=0;
	for(i=0;i < 32; i++){
		if(mBulkData[i].iID == id && mBulkData[i].iAddr == addr){
			return mBulkData[i].iData[0];
		}

	}
	return 0;
}
uint16 DynamixelPro::getBulkWord(int id, int addr){
	int i=0;
	for(i=0;i < 32; i++){
		if(mBulkData[i].iID == id && mBulkData[i].iAddr == addr){
			return DXL_MAKEWORD(mBulkData[i].iData[0], mBulkData[i].iData[1]);
		}

	}
	return 0;
}
int DynamixelPro::getBulkDword(int id, int addr){

	int i=0;
	for(i=0;i < 32; i++){
		if(mBulkData[i].iID == id && mBulkData[i].iAddr == addr){
			return  DXL_MAKEDWORD(	DXL_MAKEWORD(mBulkData[i].iData[0],mBulkData[i].iData[1]),
    								DXL_MAKEWORD(mBulkData[i].iData[2],mBulkData[i].iData[3]) );
		}

	}
	return 0;

}
int DynamixelPro::bulkWrite(byte *param, int param_length){
	 mResult = 0;
	 //uint32 bulkReadlength=0;
	 //int result=0;
	 int n;
	 //int num = param_length / 5; // each length : 5 (ID ADDR_L ADDR_H LEN_L LEN_H)
	   // int pkt_length = param_length + 3;  // 3 : INST CHKSUM_L CHKSUM_H
	 //   unsigned char txpacket[MAXNUM_TXPACKET] = {0};
	 //   unsigned char rxpacket[MAXNUM_RXPACKET] = {0};

	for(n=0; n < param_length; n++){
		gbpParameterEx[n] = param[n];

	}
	 mResult = txrx_PacketEx(BROADCAST_ID, INST_BULK_WRITE_EX, param_length);;

	return mResult;
}
byte DynamixelPro::getResult(void){
	return mResult;
}
