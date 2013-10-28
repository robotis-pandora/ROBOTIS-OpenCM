//#include "includes.h"
#include "motion.h"
#include "dxl.h"
#include "dxl_constants.h"



volatile byte gbStopPlay,gbBusUsed;
volatile byte gbMotionPlaying;
byte gbFirstDrivingStart;
word gwPlayPage,gwLastPlayPage;


byte gbpPlayHeader[HEADER_SIZE+1];
byte gbpValidServo[SERVO_NUM+1];

word gwpOffset[SERVO_NUM+1];
word gwpAdjustOffset[SERVO_NUM+1];

byte gbPageStepCount = 0;

byte gbpPageBuffer[PAGE_SIZE+HEADER_SIZE+MOTION_DATA_INTERVAL*2+1];
word gwEditPage=0;

word gwRangeBitMask[SERVO_NUM+1];
byte gbErrorPrint, gbPacketPrint;

word gwAdjustedGoalPosition;
void DriveStart(word wPlayPage)
{
	//  WriteWord(BROADCASTING_ID,P_GOAL_SPEED_L,0);
	//TxDUsbStr("DriveStart");
	byte bID;

	for(bID = 0; bID < SERVO_NUM; bID++)
	{
		if(gbpValidServo[bID]) {
			if (gwpAdjustOffset[bID] != 1024) {
				dxl_write_word(bID, P_GOAL_SPEED_L, 0);
			}
		}
	}

	gwPlayPage = wPlayPage;
	//  gbMotionPlaying = gbFirstDrivingStart = 0x55;
	gbMotionPlaying = gbFirstDrivingStart = 1;
	gbStopPlay = 0;
	gbPageStepCount = 0;



	#ifdef TEST00
	while(gbMotionPlaying)
	{
		TxD8('^');
		MotionProcessInterrupt();
		MiliSec(7);
	}
	return;
	#endif


}
/*void SetAndDisplaySensorInfomation(byte bID, byte bPrint)
{

	gbpValidSensor[bID-SENSOR_ID_START] |= (dxl_read_byte(bID,P_RETURN_LEVEL)<<5);
	if(dxl_read_byte(bID,P_RETURN_DELAY_TIME) != 0) dxl_write_byte(bID,P_RETURN_DELAY_TIME,0);

	word wModelNum = dxl_read_word(bID,P_MODEL_NUMBER_L);
	if (bPrint) {TxD8('[');TxD8Mode(bID);TxD8(':');TxD8Mode(wModelNum);}
	switch(wModelNum) {
		case 13  : gbpValidSensor[bID] |= VALID_SENSOR_AXS1 	; if(bPrint) {  TXD_STRING(STRING508); /*"(AXS1 )]")} break;
		case 208 : gbpValidSensor[bID] |= VALID_SENSOR_FSR   	; if(bPrint) { TXD_STRING(STRING509); /*"(FSR  )]")} break;
		case 211 : gbpValidSensor[bID] |= VALID_SENSOR_GPIO  	; if(bPrint) { TXD_STRING(STRING510); /*"(GPIO )]")} break;
		case 212 : gbpValidSensor[bID] |= VALID_SENSOR_BAL   	; if(bPrint) { TXD_STRING(STRING511); /*"(BAL  )]")} break;
		case 330 : gbpValidSensor[bID] |= VALID_SENSOR_IRARRAY  ; if(bPrint) { TXD_STRING(STRING519); /*"(IRARY)]")} break;
		default  : if(bPrint) { TXD_STRING(STRING507); /*"UNKWN)]"} break;
	}
}*/
word ReadServo(byte bID,byte bPosition)
{
	word wPosition;
	wPosition = dxl_read_word(bID,bPosition);
	//if(wPosition == 0xffff) return INVALID_SERVO_BIT_MASK;
	//if(wPosition == 0xffff) return 0x200;
	if(wPosition == 0xffff) return INVALID_SERVO_BIT_MASK;

	if(gwEditPage) wPosition -= gwpOffset[bID]/* + gwpAdjustOffset[bID]*/;

	if(wPosition&0x8000) wPosition = 0;
	else if(wPosition> RANGE_BIT_MASK(bID)) wPosition = RANGE_BIT_MASK(bID);
	return (wPosition);
}
word ReadServo3(byte bID,byte bAddress, byte bOffsetEnable)
{
	word wPosition;
	wPosition = dxl_read_word(bID,bAddress);
	//if(wPosition == 0xffff) return INVALID_SERVO_BIT_MASK;
	//if(wPosition == 0xffff) return 0x200;
	if(wPosition == 0xffff) return INVALID_SERVO_BIT_MASK;

	//;;
	if(bOffsetEnable) wPosition -= gwpOffset[bID]/* + gwpAdjustOffset[bID]*/;

	if(wPosition&0x8000) wPosition = 0;
	else if(wPosition> RANGE_BIT_MASK(bID)) wPosition = RANGE_BIT_MASK(bID);
	return (wPosition);
}
void SetAndDisplayServoInfomation(byte bID, byte bPrint)
{

    gbpValidServo[bID] |= (dxl_read_byte(bID,P_RETURN_LEVEL)<<5);
    if(dxl_read_byte(bID,P_RETURN_DELAY_TIME) != 0) dxl_write_byte(bID,P_RETURN_DELAY_TIME,0);

    if(dxl_read_byte(bID,P_TORQUE_ENABLE) == 1) GWP_GOAL_POSITION(bID) = ReadServo(bID,P_GOAL_POSITION_L);
    else GWP_GOAL_POSITION(bID) = ReadServo(bID,P_PRESENT_POSITION_L)|TORQUE_OFF_BIT_MASK;

    word wModelNum = dxl_read_word(bID,P_MODEL_NUMBER_L);
    word wVersion =  dxl_read_byte(bID,P_VERSION); // ydh added 20120105


    //if (bPrint) {TxD8('[');TxD8Mode(bID);TxD8(':');TxD8Mode(wModelNum);}
	switch(wModelNum) {
		case 116 : gbpValidServo[bID] |= VALID_SERVO_DX116; if(bPrint) {/* TXD_STRING(STRING501); /*"DX116)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_1023; break;
		case 117 : gbpValidServo[bID] |= VALID_SERVO_DX117; if(bPrint) { /*TXD_STRING(STRING502); /*"DX117)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_1023; break;
		case 12  : gbpValidServo[bID] |= VALID_SERVO_AX12; if(bPrint) { /*TXD_STRING(STRING503); /*"AX012)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_1023; break;
		case 28  : gbpValidServo[bID] |= VALID_SERVO_RX28; if(bPrint) { /*TXD_STRING(STRING504); /*"RX028)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_1023; break;
		case 64  : gbpValidServo[bID] |= VALID_SERVO_RX64; if(bPrint) { /*TXD_STRING(STRING505); /*"RX064)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_1023; break;
		case 106 : gbpValidServo[bID] |= VALID_SERVO_EX106; if(bPrint) { /*TXD_STRING(STRING506); /*"EX106)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_4095; break;
		case 107 : gbpValidServo[bID] |= VALID_SERVO_EX106P; if(bPrint) { /*TXD_STRING(STRING512); /*"EX106)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_4095; break;
		case 24  : gbpValidServo[bID] |= VALID_SERVO_RX24F; if(bPrint) { /*TXD_STRING(STRING513); /*"RX24F)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_1023; break;
		case 18  : gbpValidServo[bID] |= VALID_SERVO_AX18F; if(bPrint) { /*TXD_STRING(STRING514); /*"AX18F)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_1023; break;
		case 29  : gbpValidServo[bID] |= VALID_SERVO_MX28;  if(bPrint) { /*TXD_STRING(STRING515); if(wVersion<LEAST_VERSION) {TXD_STRING(STRING527);} TXD_STRING(STRING528);/*"(MX28)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_4095; break;  ///gbMX_Exist = 1; // deleted 20111228 // ydh added 20120105 if(wVersion<LEAST_VERSION) {TXD_STRING(STRING527);}
		case 300 : gbpValidServo[bID] |= VALID_SERVO_AX12W;  if(bPrint) { /*TXD_STRING(STRING518); /*"(AX12W)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_1023;  break;
		case 310  : gbpValidServo[bID] |= VALID_SERVO_MX64;  if(bPrint) { /*TXD_STRING(STRING516); if(wVersion<LEAST_VERSION) {TXD_STRING(STRING527);} TXD_STRING(STRING528); /*"(MX64)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_4095;  break; ///gbMX_Exist = 1; // deleted 20111228  // ydh added 20120105 if(wVersion<LEAST_VERSION) {TXD_STRING(STRING527);}
		case 320  : gbpValidServo[bID] |= VALID_SERVO_MX106;  if(bPrint) { /*TXD_STRING(STRING517); if(wVersion<LEAST_VERSION) {TXD_STRING(STRING527);} TXD_STRING(STRING528); /*"(MX106)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_4095;  break;///gbMX_Exist = 1; // deleted 20111228  // ydh added 20120105 if(wVersion<LEAST_VERSION) {TXD_STRING(STRING527);}
		case 350  : gbpValidServo[bID] |= VALID_SERVO_XL320; if(bPrint) { /*TXD_STRING(STRING529); /*"(XL320)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_1023; break;
		default  : if(bPrint) { /*TXD_STRING(STRING507); /*"UNKWN)]"*/} RANGE_BIT_MASK(bID) = RANGE_BIT_MASK_4095; break;
	}
}

byte ValidServoCheckSetOffset(void)
{
	byte bCount, bID;
	unsigned long ulFlashPointer;


	bCount = 0;
	for(bID = 0; bID < 27; bID++)
	{
		#ifdef __BOARD_TYPE_MODULE__485_TTL_BOTH
		UART0_RS485;
		#endif
		if(txrx_Packet(bID,INST_PING,0))
		{

			if(bID < SERVO_NUM) {
				//gbpValidServo[bID] = 0x80;
				SetAndDisplayServoInfomation(bID, 1);
			}
			/*else if(SENSOR_ID_START <= bID&&bID <= SENSOR_ID_END) {
				//gbpValidSensor[bID-SENSOR_ID_START] = 0x80;
				//SetAndDisplaySensorInfomation(bID, 1);
			}
			else {
				//DisplayServoInfomation(bID, bPrint);
			}*/
			bCount++;
		}
		else
		{
			#ifndef __BOARD_TYPE_MODULE__485_TTL_BOTH
			if(bID < SERVO_NUM) gbpValidServo[bID] = 0;
			//else if(SENSOR_ID_START <= bID&&bID <= SENSOR_ID_END) gbpValidSensor[bID-SENSOR_ID_START] = 0;
			#endif
		}
	}

	ulFlashPointer = MOTION_DATA_START_ADDRESS+HEADER_SIZE+MOTION_DATA_INTERVAL*2;

	for(bID = 0; bID < SERVO_NUM; bID++)
	{
		gwpOffset[bID] = (((word)READ_FLASH(ulFlashPointer+(bID<<1)+1))<<8)+READ_FLASH(ulFlashPointer+(bID<<1));
		gwpAdjustOffset[bID] = 0;
		if(gwpOffset[bID] == INVALID_SERVO_BIT_MASK || gwpOffset[bID] == INVALID_SERVO_BIT_MASK) gwpOffset[bID] = 0;
	}

	//if(bPrint) {TXD_STRING(CR_LF_);TxD8('}');}

	return bCount;
}

void MotionProcessInterrupt(void){


	byte bID,bPageNextStep, bNextPlayPage;//, bRepeat;
	unsigned long ulTotalTime256T, ulPreSectionTime256T,ulMainTime256T;
	long lStartSpeed1024_PreTime_256T,lMovingAngle_Speed1024Scale_256T_2T,lDivider1,lDivider2;
	word wCount,wMaxAngle1024,wMaxSpeed256,wTmp;
	int iSpeedN;
	static unsigned long ulFlashPointer;
	static int ipMovingAngle1024[SERVO_NUM+1];
	static int ipMainAngle1024[SERVO_NUM+1];
	static int ipAccelAngle1024[SERVO_NUM+1];
	static word wpStartAngle1024[SERVO_NUM+1];
	//  static word wpServoValue[SERVO_NUM+2];
	static word wpTargetAngle1024[SERVO_NUM+1];
	static int ipMainSpeed1024[SERVO_NUM+1];
	static int ipLastOutSpeed1024[SERVO_NUM+1];
	static int ipGoalSpeed1024[SERVO_NUM+1];
	static byte bpFinishType[SERVO_NUM+1];
	static word wUnitTimeCount,wUnitTimeNum,wPauseTime,wUnitTimeTotalNum, wAccelStep;//,wDataPointer;
	static byte bSection/*gbPageStepCount,*/, bMotionFinish=0;
	enum {PRE_SECTION,MAIN_SECTION,POST_SECTION,PAUSE_SECTION,PAGE_PAUSE_SECTION};
	enum {ZERO_FINISH,NONE_ZERO_FINISH};

	if(gbFirstDrivingStart){

	  	bMotionFinish = 0;
		wPauseTime = wUnitTimeCount = wUnitTimeNum = 0;
		gbpPlayHeader[P_REPEAT_COUNT] = 0;

		bSection = PAUSE_SECTION;
		for(bID = 0; bID < SERVO_NUM; bID++){

			wpTargetAngle1024[bID] = GWP_GOAL_POSITION(bID);
			ipLastOutSpeed1024[bID] = ipMovingAngle1024[bID] = ipGoalSpeed1024[bID] = 0;

		}

	}
	//Section             /----\
	//                   /|    |\
	//       /+---------/ |    | \
	//      / |        |  |    |  \
	//-----/  |        |  |    |   \----
	//     PRE  MAIN   PRE MAIN POST PAUSE
	/*  051030단번에 끝내자
	if(bMotionFinish)
	{
	//050723
	//if(RECENT_INSTRUCTION != INST_ACTION)
	//{ //???pause time없이 끝날때만 해줘야....큰문제는 유발하지 않을 듯...
	//  TxPacket(BROADCASTING_ID,INST_ACTION,0);
	// }
	gbMotionPlaying = 0;
	return;
	}
	*/

	if(wUnitTimeCount < wUnitTimeNum ){
		//TxDByteC('$');
		wUnitTimeCount++;
		#define WEIGHT_HISTORY_PROCESS 0x01
		#define XY_BALANCE_PROCESS 0x02
		#define ERROR_OFFSET_PROCESS 0x04

		if(bSection == PAUSE_SECTION /*|| bSection == PAGE_PAUSE_SECTION*/){

		}else{
			byte bPointer;
			#ifdef SPEED_CONTROL
			word wOutSpeed;
			#endif

			bPointer = 0;

			//ss051030 gbpParameter[bPointer++] = P_GOAL_POSITION_L;// CW_COMPLIANCE_SLOPE; //ss040817
			gbpParameter[bPointer++] =  P_CW_COMPLIANCE_SLOPE; ////ss051030 // also P_P_GAIN 28.
			#ifdef SPEED_CONTROL
			gbpParameter[bPointer++] = 4;
			#else
			//ss051030 gbpParameter[bPointer++] = 2;//4->2 ss040817
			gbpParameter[bPointer++] = 4;////ss051030
			#endif

			for(bID = 0; bID < SERVO_NUM; bID++){
				//        if(gbpValidServo[bID] && (bpMotionEnable[bID]==1))
				if(gbpValidServo[bID] /*&& (wpServoValue[bID]&0x8000==0)*/){
					if (((WORD_CAST(gbpPageBuffer[MOTION_BUFFER_1+(bID<<1)]) &
						(INVALID_SERVO_BIT_MASK|TORQUE_OFF_BIT_MASK))!=0) 	 ||
						(gwpAdjustOffset[bID] == 1024)){

						continue;
					}


					if(bSection == PRE_SECTION){
						iSpeedN = (int)(((long)(ipMainSpeed1024[bID]-ipLastOutSpeed1024[bID])*wUnitTimeCount) / wUnitTimeNum);
						ipGoalSpeed1024[bID] = ipLastOutSpeed1024[bID] + iSpeedN;

						ipAccelAngle1024[bID] = ((((long)(ipLastOutSpeed1024[bID] + (iSpeedN>>1))*wUnitTimeCount*144)/15)>>9);

						GWP_GOAL_POSITION(bID) = wpStartAngle1024[bID] + ipAccelAngle1024[bID];

					}
					else if(bSection == MAIN_SECTION)
					{
						GWP_GOAL_POSITION(bID) = wpStartAngle1024[bID] + (int)(((long)(ipMainAngle1024[bID])*wUnitTimeCount) / wUnitTimeNum);

						ipGoalSpeed1024[bID] = ipMainSpeed1024[bID];
					}
					else //if(bSection == POST_SECTION)
					{
						if(bpFinishType[bID]==ZERO_FINISH)
						{ //감속해서 속도를 0으로
							iSpeedN = (int)(((long)(0-ipLastOutSpeed1024[bID])*wUnitTimeCount) / wUnitTimeNum);
							ipGoalSpeed1024[bID] = ipLastOutSpeed1024[bID] + iSpeedN;

							GWP_GOAL_POSITION(bID) = wpStartAngle1024[bID] + ((((long)(ipLastOutSpeed1024[bID] + (iSpeedN>>1))*wUnitTimeCount*144)/15)>>9);

						}
						else
						{//MAIN Section과 동일하게 작동-동일 step에서 어떤서보는 가고 어떤 서보는 서야하는 상황이 발생할 수 있으므로 이렇게 할수밖에없음
							GWP_GOAL_POSITION(bID) = wpStartAngle1024[bID] + (int)(((long)(ipMainAngle1024[bID])*wUnitTimeCount) / wUnitTimeNum);

							ipGoalSpeed1024[bID] = ipMainSpeed1024[bID];
						}
					}
					//;;;;;; GWP_GOAL_POSITION(bID) += gipComp[bID];

					if(GWP_GOAL_POSITION(bID)&0x8000) GWP_GOAL_POSITION(bID) = 0;
					else if(GWP_GOAL_POSITION(bID) > RANGE_BIT_MASK(bID)) GWP_GOAL_POSITION(bID) = RANGE_BIT_MASK(bID);


					gbpParameter[bPointer++] = bID;

					//ydh added 20111228 ver 1.1968
					if( 	((gbpValidServo[bID] & 0x7F) == VALID_SERVO_MX28 ) ||
							((gbpValidServo[bID] & 0x7F) == VALID_SERVO_MX64 ) ||
							((gbpValidServo[bID] & 0x7F) == VALID_SERVO_MX106 ) )
					{
						const byte bpSoftness[8] = { 254, 254, 254, 128, 64, 32, 16, 8 };

						gbpParameter[bPointer++] = bpSoftness[ (gbpPlayHeader[P_COMPLIANCE_IN_HEADER+bID]&0x0f) ] ; //ydh 20110908
						gbpParameter[bPointer++] = 0;

					}
					else
					{
						gbpParameter[bPointer++] = 1<<(gbpPlayHeader[P_COMPLIANCE_IN_HEADER+bID]>>4); //051030
						gbpParameter[bPointer++] = 1<<(gbpPlayHeader[P_COMPLIANCE_IN_HEADER+bID]&0x0f); //051030
					}


					/* original code 1.1967

					if( gbMX_Exist )
					{
						const byte bpSoftness[8] = { 254, 254, 254, 128, 64, 32, 16, 8 };

						gbpParameter[bPointer++] = bpSoftness[ (gbpPlayHeader[P_COMPLIANCE_IN_HEADER+bID]&0x0f) ] ; //ydh 20110908
						gbpParameter[bPointer++] = 0;
						gbpParameter[bPointer++] = 0;
						gbpParameter[bPointer++] = 0;


					}
					else
					{
						gbpParameter[bPointer++] = 1<<(gbpPlayHeader[P_COMPLIANCE_IN_HEADER+bID]>>4); //051030
						gbpParameter[bPointer++] = 1<<(gbpPlayHeader[P_COMPLIANCE_IN_HEADER+bID]&0x0f); //051030
					}*/

					gwAdjustedGoalPosition = gwpAdjustOffset[bID] + GWP_GOAL_POSITION(bID);
					if(gwAdjustedGoalPosition&0x8000) gwAdjustedGoalPosition = 0;
					else if(gwAdjustedGoalPosition > RANGE_BIT_MASK(bID)) gwAdjustedGoalPosition = RANGE_BIT_MASK(bID);

					gbpParameter[bPointer++] = gwAdjustedGoalPosition&0xff;
					gbpParameter[bPointer++] = (gwAdjustedGoalPosition>>8)&0xff;
					#ifdef SPEED_CONTROL
					if(ipGoalSpeed1024[bID] < 0) wOutSpeed = -ipGoalSpeed1024[bID];
					else wOutSpeed = ipGoalSpeed1024[bID];
					if(wOutSpeed == 0) wOutSpeed = 1;
					wOutSpeed += 0x20;
					if(wOutSpeed > RANGE_BIT_MASK) wOutSpeed = 0;
					gbpParameter[bPointer++] = wOutSpeed&0xff;
					gbpParameter[bPointer++] = (wOutSpeed>>8)&0xff;
					//          gbpParameter[bPointer++] = 0;
					//          gbpParameter[bPointer++] = 0;
					#endif
				}
			}
			/*************************************************************************************************
			 ***************************  Send packet to Dynamixel BUS  **************************************
			 * ***********************************************************************************************
			 * */
			tx_Packet(BROADCAST_ID,INST_SYNC_WRITE,bPointer);
		}
		#ifdef DEBUG_STEP
		TXD_STRING(CR_LF_);
		TxD32Dec(wpStartAngle1024[0x12]);TxD8(' ');
		TxD32Dec(GWP_GOAL_POSITION(0x12));TxD8(',');
		TxD32Dec(ipGoalSpeed1024[0x12]);TxD8(' ');
		TxD32Dec(ipLastOutSpeed1024[0x12]);TxD8(',');
		TxD32Dec(wUnitTimeCount);TxD8(' ');
		TxD32Dec(wUnitTimeNum);TxD8(' ');
		#endif

	}
	//else if로 하거나 위의 블럭과 순서를 바꾸어서는 안된다.(읽는 시점이 여유있는 상태가 되야)
	if(bMotionFinish)
	  gbMotionPlaying = 0;
	else if(wUnitTimeCount >= wUnitTimeNum){

		wUnitTimeCount = 0;
		#ifdef DEBUG_SECTION
		TxDUsbStr(CR_LF_);
		#endif
		for(bID = 0; bID < SERVO_NUM; bID++){
			wpStartAngle1024[bID] = GWP_GOAL_POSITION(bID);
			ipLastOutSpeed1024[bID] = ipGoalSpeed1024[bID];
		}
		if(bSection == PRE_SECTION){
		  bSection = MAIN_SECTION;
			if (wUnitTimeTotalNum==1) bSection = POST_SECTION;
			wUnitTimeNum =  wUnitTimeTotalNum - (wAccelStep<<1);

			for(bID = 0; bID < SERVO_NUM; bID++){
				// // 0 / 0의 상황이 발생하여 프로그램이 죽음 ;;;;;;;;; after ver 1.15ab
				// if(bpFinishType[bID] == NONE_ZERO_FINISH) ipMainAngle1024[bID] = (((long)(ipMovingAngle1024[bID] - ipAccelAngle1024[bID]))*wUnitTimeNum)/(wUnitTimeTotalNum - wAccelStep);
				if(bpFinishType[bID] == NONE_ZERO_FINISH){
					if ((wUnitTimeTotalNum - wAccelStep) == 0) {
						ipMainAngle1024[bID] = 0;
					}
					else {
						ipMainAngle1024[bID] = (((long)(ipMovingAngle1024[bID] - ipAccelAngle1024[bID]))*wUnitTimeNum)/(wUnitTimeTotalNum - wAccelStep);
					}

				}else {
					ipMainAngle1024[bID] = ipMovingAngle1024[bID] - ipAccelAngle1024[bID] - (int)((((long)ipMainSpeed1024[bID]*wAccelStep*12)/5)>>8);
				}


			}
#ifdef DEBUG_SECTION
			TxDUsbStr("[MAIN Start]");TxDUsbHex32(bpFinishType[0x12]);TxDUsbHex32(wAccelStep);
			TxDUsbStr(" MnSpd:");TxDUsbHex32(ipMainSpeed1024[0x12]);
			TxDUsbStr(" MovA:");TxDUsbHex32(ipMovingAngle1024[0x12]);
			TxDUsbStr(" Acc:");TxDUsbHex32(ipAccelAngle1024[0x12]);
			TxDUsbStr(" MnAn:");TxDUsbHex32(ipMainAngle1024[0x12]);
#endif

		}else if(bSection == MAIN_SECTION){
			bSection = POST_SECTION;
#ifdef DEBUG_SECTION
			TxDUsbStr("[POST Start]");
#endif

			wUnitTimeNum = wAccelStep;
			for(bID = 0; bID < SERVO_NUM; bID++){
				ipMainAngle1024[bID] = ipMovingAngle1024[bID] - ipMainAngle1024[bID] - ipAccelAngle1024[bID];
			}
		}else if(bSection == POST_SECTION){
			if(wPauseTime)
			{
				bSection = PAUSE_SECTION;
#ifdef DEBUG_SECTION
				TxDUsbStr("[PAUSE Start]");
#endif
				wUnitTimeNum = wPauseTime;
			}
			else
			{
				bSection = PRE_SECTION; //read step!
#ifdef DEBUG_SECTION
				TxDUsbStr("[PRE Start]");
#endif
			}
		}else if(bSection == PAUSE_SECTION){
			for(bID = 0; bID < SERVO_NUM; bID++)  // same routine to FirstDriving
			{
				wpTargetAngle1024[bID] = GWP_GOAL_POSITION(bID);
				ipLastOutSpeed1024[bID] = ipMovingAngle1024[bID] = ipGoalSpeed1024[bID] = 0;
			}
			bSection = PRE_SECTION; //read step!
		}

		//READ_DATA0:
		if(bSection == PRE_SECTION ){
			gbPageStepCount++;

			if(gbPageStepCount >= gbpPlayHeader[P_PAGE_STEP] || gbFirstDrivingStart) //ss040309 == -> >=
			{//Read Page Data
			//gwPlayPage++;; ss040310
				if(!gbFirstDrivingStart)
				{
					SET_PLAY_PAGE:

					gwLastPlayPage = gwPlayPage;
					if(gbpPlayHeader[P_REPEAT_COUNT]){
						gbpPlayHeader[P_REPEAT_COUNT]--; //ss051030
					}

					if(gbpPlayHeader[P_REPEAT_COUNT]);   //What is this????

					else if (gbStopPlay == STOP_IN_THIS_PAGE){
						gwPlayPage = 0;
					}
					else if(gbStopPlay == STOP_VIA_EXIT_PAGE){
						gwPlayPage = gbpPlayHeader[P_EXIT_PAGE];
					}
					else{
						gwPlayPage = gbpPlayHeader[P_NEXT_PAGE];
					}

				}
				if(!gwPlayPage) bMotionFinish = 1;
				else{
					//gwPlayPage = (word)bTmpPlayPage; //maintion gwPlayPage for reading
					gbFirstDrivingStart = 0;  //First Process end

#ifdef DEBUG_SECTION
					TxDStringC("\r\n [PageRD]");TxDHex16C(gwPlayPage);
#endif
					if(gwEditPage == gwPlayPage){
						//TxDStringC("@");
						ulFlashPointer = 0;
						if(!gbpPlayHeader[P_REPEAT_COUNT]){
							for(wCount = P_PAGE_NAME_END; wCount < HEADER_SIZE;wCount++){//Header읽기
								gbpPlayHeader[wCount] = gbpPageBuffer[ulFlashPointer+wCount];
							}
							//TxD8('R');;TxD8Hex(gbpPlayHeader[P_REPEAT_COUNT]);
						}
					}
					else{

						ulFlashPointer = MOTION_DATA_START_ADDRESS+(((unsigned long)gwPlayPage)<<9);

						if(!gbpPlayHeader[P_REPEAT_COUNT]){

							for(wCount = P_PAGE_NAME_END; wCount < HEADER_SIZE;wCount++){//Header읽기
								gbpPlayHeader[wCount] = READ_FLASH(ulFlashPointer+wCount);
							}
						}
					}
					//After reading header, increase flash pointer
					ulFlashPointer += HEADER_SIZE;

					if(gbpPlayHeader[P_PAGE_STEP] > MAX_STEP_NUM || gbpPlayHeader[P_PAGE_STEP] == 0 )
					{ //invalid page
						//TXD_STRING(STRING304); //"[Invalid Page Read:"
						//TxD16Hex(gwPlayPage);TxD8(']');
						bMotionFinish = 1;
					}
					//wDataPointer = HEADER_SIZE;
					if(!gbpPlayHeader[P_REPEAT_COUNT]) goto SET_PLAY_PAGE;
						gbPageStepCount = 0;//Read Process
				}

			}
			//Read Currrent Step 매번 64byte에 신규 데이터를 읽는다.
			for(wCount = 0; wCount < MOTION_DATA_INTERVAL;wCount++){
				if(gwEditPage == gwPlayPage){
					if( ulFlashPointer >= (PAGE_SIZE+HEADER_SIZE+MOTION_DATA_INTERVAL*2+1) ) break; //ydh added 20120220 for debug.
					gbpPageBuffer[MOTION_BUFFER_1+wCount] = gbpPageBuffer[ulFlashPointer++]; //RAM에서 실행

				}
				else{
					//TxDStringC("\r\n ulFlashPointer = "); TxDHex32C(ulFlashPointer);
					gbpPageBuffer[MOTION_BUFFER_1+wCount] = READ_FLASH(ulFlashPointer++);

				}
			}
			//Read Next Step 매번 64byte에 신규 데이터를 읽는다. 가감속을 위해서는 다음 번 데이터를 알아야 한다.
			bPageNextStep = gbPageStepCount+1;
			bNextPlayPage = (byte)gwPlayPage;

			//TxDStringC("\r\n gbpPlayHeader[P_PAGE_STEP] = "); TxDHex16C(gbpPlayHeader[P_PAGE_STEP]);
			if(bPageNextStep >= gbpPlayHeader[P_PAGE_STEP]) //Check if motion play is finished?
			{
				if (gbStopPlay == STOP_IN_THIS_PAGE){
					gwPlayPage = 0;
				}
				else if(gbStopPlay == STOP_VIA_EXIT_PAGE) {
					gbpPlayHeader[P_REPEAT_COUNT] = 0;
					bNextPlayPage = gbpPlayHeader[P_EXIT_PAGE];
				}
				else if(gbpPlayHeader[P_REPEAT_COUNT] > 1){
					bNextPlayPage = (byte)gwPlayPage;
				}
				else {
					bNextPlayPage = gbpPlayHeader[P_NEXT_PAGE];
				}

				//if(gwEditPage == gwPlayPage)
				if(bNextPlayPage == gwEditPage)
				{
					ulFlashPointer = HEADER_SIZE;
				}
				else
				{
					if(bNextPlayPage == 0){
						ulFlashPointer -= MOTION_DATA_INTERVAL; // 끝이면 자기 자신을 한번 더 읽는 것으로 처리
					}
					else {
						ulFlashPointer = MOTION_DATA_START_ADDRESS+ (((unsigned long)bNextPlayPage)<<9) + HEADER_SIZE;
					}
				}
			}
			for(wCount = 0; wCount < MOTION_DATA_INTERVAL;wCount++)
			{
				//gbpPageBuffer[HEADER_SIZE+MOTION_DATA_INTERVAL+wCount] = READ_FLASH(ulFlashPointer+wCount);
				if(bNextPlayPage == (byte)gwEditPage) gbpPageBuffer[MOTION_BUFFER_2+wCount] = gbpPageBuffer[ulFlashPointer+wCount]; //RAM에서 실행
				else gbpPageBuffer[MOTION_BUFFER_2+wCount] = READ_FLASH(ulFlashPointer+wCount);
				//TxD8Hex(gbpPageBuffer[MOTION_BUFFER_2+wCount]);;;;;
			}

			//     if(gbPageStepCount >= GB_PAGE_STEP)// && bSection == PAUSE_SECTION)
			//    { //Normal finish
			//     bMotionFinish = 1;
			//  }      //    <---------\  <-----\ -------\  //PageStep은 맨마지막단계에서 증가시킨다.
			if(!bMotionFinish )   //  PRE -> MAIN (-> POST -> PAUSE)
			{      //  모든 데이터는 PreSection진입바로 전에 읽는다.
				#ifdef DEBUG_SECTION
				TxDUsbStr("[StepRD]");
				#endif

				wPauseTime = (((word)gbpPageBuffer[MOTION_BUFFER_1+(MOTION_DATA_INTERVAL-2)])<<5)/gbpPlayHeader[P_PAGE_SPEED];
				wMaxSpeed256 = ((word)gbpPageBuffer[MOTION_BUFFER_1+(MOTION_DATA_INTERVAL-2+1)]*(word)gbpPlayHeader[P_PAGE_SPEED])>>5;

				if(wMaxSpeed256 == 0)
				{
					wMaxSpeed256 = 1; //TIME BASE 051025

				}

				wMaxAngle1024 = 0;
				for(bID = 0; bID < SERVO_NUM; bID++)
				{
					word wPrevTargetAngle, wCurrentTargetAngle,wNextTargetAngle;
					byte bDirectionChanged;
					if(gbpValidServo[bID])
					//          if(gbpValidServo[bID] && (((wpTargetAngle1024[bID]&(0x8000|INVALID_SERVO_BIT_MASK|TORQUE_OFF_BIT_MASK))==0)))
					{

						if (((WORD_CAST(gbpPageBuffer[MOTION_BUFFER_1+(bID<<1)])&(INVALID_SERVO_BIT_MASK|TORQUE_OFF_BIT_MASK))!=0) || (gwpAdjustOffset[bID] == 1024))
						{
							continue;
						}

						wpStartAngle1024[bID] = GWP_GOAL_POSITION(bID) = wpTargetAngle1024[bID]; //한step마다 시작점을 초기화한다.
						ipAccelAngle1024[bID] = 0;

						//wpServoValue[bID] = wCurrentTargetAngle = ((word)READ_FLASH(ulFlashPointer+(bID<<1)+1)<<8) + READ_FLASH(ulFlashPointer+(bID<<1));
						wCurrentTargetAngle = WORD_CAST(gbpPageBuffer[MOTION_BUFFER_1+(bID<<1)]);//&RANGE_BIT_MASK;

						//ssing040530 wpServoValue[bID] = wCurrentTargetAngle = WORD_CAST(gbpPageBuffer[wDataPointer+(bID<<1)]);
						//wpServoValue is only for memorizing compliance
						wCurrentTargetAngle += (gwpOffset[bID]/*+gwpAdjustOffset[bID]*//*+(signed short)gcpLoadError[bID]*/);
						//wCurrentTargetAngle += gipComp[bID]; //SS040830
						if(wCurrentTargetAngle&0x8000) wCurrentTargetAngle = 0;
						else if(wCurrentTargetAngle > RANGE_BIT_MASK(bID)) wCurrentTargetAngle = RANGE_BIT_MASK(bID);

						ipMovingAngle1024[bID] = (int)(wCurrentTargetAngle - wpTargetAngle1024[bID]);

						wPrevTargetAngle = wpTargetAngle1024[bID];
						wpTargetAngle1024[bID] = wCurrentTargetAngle; //target angle update

						wNextTargetAngle = ((WORD_CAST(gbpPageBuffer[MOTION_BUFFER_1+(bID<<1)+MOTION_DATA_INTERVAL]))) + gwpOffset[bID]/* + (signed short)gcpLoadError[bID]*/ /*+ gwpAdjustOffset[bID]*/;
						if(wNextTargetAngle&0x8000) wNextTargetAngle = 0;
						else if(wNextTargetAngle > RANGE_BIT_MASK(bID)) wNextTargetAngle = RANGE_BIT_MASK(bID);
						////////////
						if((wPrevTargetAngle < wCurrentTargetAngle && wCurrentTargetAngle < wNextTargetAngle)
						|| (wPrevTargetAngle > wCurrentTargetAngle && wCurrentTargetAngle > wNextTargetAngle)) bDirectionChanged = 0;
						else bDirectionChanged = 1;

						if(bDirectionChanged || wPauseTime || (gbPageStepCount >= gbpPlayHeader[P_PAGE_STEP]-1 && bNextPlayPage == 0))
						//            if(bDirectionChanged || wPauseTime || (gbPageStepCount >= gbpPlayHeader[P_PAGE_STEP]-1 && bNextPlayPage == 0))
						//            if(bDirectionChanged )
						{
							//TxD8('-');;
							bpFinishType[bID] = ZERO_FINISH;

						}
						else
						{
							//TxD8Hex_(bID);;
							bpFinishType[bID] = NONE_ZERO_FINISH;
						}

						if(gbpPlayHeader[P_CONTROL_SCHEDULE] != TIME_BASE)
						{
							//MaxAngle1024 update
							if(ipMovingAngle1024[bID] < 0) wTmp = -ipMovingAngle1024[bID];
							else wTmp = ipMovingAngle1024[bID];
							if(wTmp > wMaxAngle1024) wMaxAngle1024 = wTmp;
						}
					}
				}
				if(gbpPlayHeader[P_CONTROL_SCHEDULE] == TIME_BASE)
					wUnitTimeTotalNum  = wMaxSpeed256; //TIME BASE 051025
				else
					wUnitTimeTotalNum  = (word)(((long)(wMaxAngle1024*40)) /((long)(wMaxSpeed256 *3)));


				wAccelStep = gbpPlayHeader[P_ACCEL_TIME];
				if(wUnitTimeTotalNum <= (wAccelStep<<1))
				{
					//TXD_STRING(STRING303);//TxDString("\r\n [Too Fast or Far to Accel :");
					if(gbErrorPrint&ACCEL_TIME_ERROR_PRINT)
					{
						/*TxD8('[');
						TxD8Hex(gbPageStepCount);
						//TxD8(',');TxD16Hex(wMaxAngle1024);
						//TxD8(',');TxD16Hex(wMaxSpeed256);
						TxD8(',');TxD16Hex(wUnitTimeTotalNum);
						TxD8(']');*/
					}
					if(wUnitTimeTotalNum == 0) wAccelStep = 0;
					else
					{
						wAccelStep = (wUnitTimeTotalNum-1)>>1;
						if(wAccelStep == 0) wUnitTimeTotalNum = 0; //움직이려면 적어도 가속,등속이 한 스텝이상씩은 존재해야
					}
				}

				if(wUnitTimeTotalNum == 0 && wAccelStep == 0) wUnitTimeTotalNum = 1;;;;

				ulTotalTime256T = ((unsigned long)wUnitTimeTotalNum)<<1;// /128 * 256
				ulPreSectionTime256T = ((unsigned long)wAccelStep)<<1;// /128 * 256
				ulMainTime256T = ulTotalTime256T - ulPreSectionTime256T;
				lDivider1 = ulPreSectionTime256T + (ulMainTime256T<<1);
				lDivider2 = (ulMainTime256T<<1);
				if(lDivider1 == 0) lDivider1 = 1;
				if(lDivider2 == 0) lDivider2 = 1;

				for(bID = 0; bID < SERVO_NUM; bID++)
				{
					lStartSpeed1024_PreTime_256T = ((long)ipLastOutSpeed1024[bID])*ulPreSectionTime256T;
													   //  *300/1024 * 1024/720 * 256 * 2
					lMovingAngle_Speed1024Scale_256T_2T = (((long)ipMovingAngle1024[bID])*2560L)/12;
					if(bpFinishType[bID] == ZERO_FINISH)
					{
						ipMainSpeed1024[bID] = (int)((lMovingAngle_Speed1024Scale_256T_2T - lStartSpeed1024_PreTime_256T)/lDivider2);
					}
					else
					{
						ipMainSpeed1024[bID] = (int)((lMovingAngle_Speed1024Scale_256T_2T - lStartSpeed1024_PreTime_256T)/lDivider1);
					}
					if(ipMainSpeed1024[bID] > 1023) ipMainSpeed1024[bID] = 1023;
					if(ipMainSpeed1024[bID] < -1023) ipMainSpeed1024[bID] = -1023;
					//          if(ipMainSpeed1024[bID] > 4095) ipMainSpeed1024[bID] = 4095;
					//          if(ipMainSpeed1024[bID] < -4095) ipMainSpeed1024[bID] = -4095;
				}

				#ifdef DEBUG_SECTION
				//        TxDString(" l MA2:");TxD32Dec(lMovingAngle_Speed1024Scale_256T_2T);
				//        TxDString(" ul Div1:");TxD32Dec(lDivider1);
				//        TxDString(" l V0:");TxD32Dec(lStartSpeed1024_PreTime_256T);
				//        TxDString(" LastSp");TxD32Dec(ipLastOutSpeed1024[1]);
				//        TxDString(" Pt245:");TxD32Dec(ulPreSectionTime256T);
				//        lStartSpeed1024_PreTime_256T = ((long)ipLastOutSpeed1024[1])*ulPreSectionTime256T;
				//        TxDString(" l V02:");TxD32Dec(lStartSpeed1024_PreTime_256T);

				TxDUsbStr(" PG:");TxDUsbHex32(gwPlayPage);
				TxDUsbStr(" STP:");TxDUsbHex32(gbPageStepCount);
				TxDUsbStr(" N:");TxDUsbHex32(bNextPlayPage);
				TxDUsbU8(',');TxDUsbHex32(bPageNextStep);
				TxDUsbStr(" MAg:");TxDUsbHex32(wMaxAngle1024);
				TxDUsbStr(" MSd:");TxDUsbHex32(wMaxSpeed256);
				//TxDString(" MvAg:");TxD32Dec(ipMovingAngle1024[1]);
				//TxDString(" MnSd:");TxD32Dec(ipMainSpeed1024[1]);
				TxDUsbStr(" TNum:");TxDUsbHex32(wUnitTimeTotalNum);
				//RxD8Interrupt();
				#endif

				wUnitTimeNum = wAccelStep; //PreSection

			}

		}

	}

}

