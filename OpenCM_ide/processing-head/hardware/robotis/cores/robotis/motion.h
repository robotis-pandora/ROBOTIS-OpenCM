#ifndef _MOTION_H
#define _MOTION_H

#ifdef __cplusplus
extern "C" {
#endif
#include "libpandora_types.h"

#define SERVO_NUM 26

#define MOTION_DATA_INTERVAL 64
#define HEADER_SIZE 64
#define PAGE_SIZE 512
#define MAX_STEP_NUM ((PAGE_SIZE-HEADER_SIZE)/MOTION_DATA_INTERVAL) //7(0~6)

#define WORD_CAST(AA) (*(word *)(&(AA)))
#define ROM_CAST(ADDR) ( (byte)(EE_ReadVariable2(ADDR)) )
#define READ_FLASH(AA) (*(byte  *)(AA))

#define RANGE_BIT_MASK(A) gwRangeBitMask[A] //0~3ff or 0~fff range
#define RANGE_BIT_MASK_1023 (1023)
#define RANGE_BIT_MASK_4095 (4095)

// Header Structure
// Name:0~16
// StepNumber 20
// Torque 21 ->modified to code page
// Speed  22
// Delay  23
// AccelTime 24
// NextPage 25
// ExitPage 26
//
// Control Margin 32~63
#define P_PAGE_NAME 0
#define P_PAGE_NAME_END 13

#define P_PAGE_PAUSE_TIME 14
#define P_REPEAT_COUNT 15
#define P_CONTROL_SCHEDULE 16

/*
#define P_BALANCE_X_AXIS 14
#define P_MINLOAD_X_PAGE       15
#define P_MAXLOAD_X_PAGE       16

#define P_BALANCE_Y_AXIS  17
#define P_MINLOAD_Y_PAGE       18
#define P_MAXLOAD_Y_PAGE       19
*/

#define P_PAGE_STEP 20
#define P_PLAY_CODE 21
#define P_PAGE_SPEED 22
#define P_DXL_SETUP 23
#define P_ACCEL_TIME 24
#define P_NEXT_PAGE 25
#define P_EXIT_PAGE 26

#define P_LINKED_PAGE1 27
#define P_LINKED_PAGE1_PLAY_CODE 28
#define P_LINKED_PAGE2 29
#define P_LINKED_PAGE2_PLAY_CODE 30


#define P_PAGE_CHECKSUM 31
#define P_CONTROL_MARGIN 32
#define P_COMPLIANCE_IN_HEADER 32

#define P_BALANCE_X 34
#define P_BALANCE_Y 35
#define P_WEIGHT 36


#define GWP_GOAL_POSITION(ADDR) WORD_CAST(gbpPageBuffer[PAGE_SIZE+(ADDR<<1)])


/*
 * Motion Finish Flags
 */
#define STOP_IN_THIS_PAGE 1
#define STOP_VIA_EXIT_PAGE 2


#define MOTION_DATA_START_ADDRESS 	0x800B000  //64K~128K : 64Kbytes
#define MOTION_DATA_END_ADDRESS 	0x8020000

#define  STRING304/*]=*/ "[Invalid Page Read:"

#define MOTION_BUFFER_1 (PAGE_SIZE+MOTION_DATA_INTERVAL)
#define MOTION_BUFFER_2 (PAGE_SIZE+MOTION_DATA_INTERVAL*2)
#define INVALID_SERVO_BIT_MASK 0x4000
#define TORQUE_OFF_BIT_MASK 0x2000

#define	TIME_BASE	0x0a // play speed during debugging  1.1964
#define ACCEL_TIME_ERROR_PRINT 4 //when motion playing [1,0000


#define VALID_SERVO_DX116   1
#define VALID_SERVO_DX117   2
#define VALID_SERVO_AX12    3
#define VALID_SERVO_RX28    4
#define VALID_SERVO_RX64    5
#define VALID_SERVO_EX106   6
#define VALID_SERVO_EX106P  7
#define VALID_SERVO_RX24F   8
#define VALID_SERVO_AX18F   9
#define VALID_SERVO_MX28	10
#define VALID_SERVO_AX12W	11
#define VALID_SERVO_MX64	12
#define VALID_SERVO_MX106	13
#define VALID_SERVO_XL320   14


void DriveStart(word wPlayPage);
void MotionProcessInterrupt(void);
word ReadServo(byte bID,byte bPosition);
word ReadServo3(byte bID,byte bAddress, byte bOffsetEnable);
//void SetAndDisplaySensorInfomation(byte bID, byte bPrint);
byte ValidServoCheckSetOffset(void);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_ENGINE_C_ */
