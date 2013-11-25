/*
 * dxl_constants.h
 *
 *  Created on: 2012. 12. 17.
 *      Author: ROBOTIS(jason@robotis.com)
 */

#ifndef DXL_CONSTANTS_H_
#define DXL_CONSTANTS_H_


#define BROADCAST_ID		(254) /* 0xFE */

/*
 * Instruction command
 * */

#define INST_PING           0x01
#define INST_READ           0x02
#define INST_WRITE          0x03
#define INST_REG_WRITE      0x04
#define INST_ACTION         0x05
#define INST_RESET          0x06
#define INST_DIGITAL_RESET  0x07
#define INST_SYSTEM_READ    0x0C
#define INST_SYSTEM_WRITE   0x0D
#define INST_SYNC_WRITE     0x83
#define INST_SYNC_REG_WRITE 0x84


/*
 * Dynamixel common control table -> see ROBOTIS support page(support.robotis.com)
 * */
#define P_MODEL_NUMBER_L      0
#define P_MODOEL_NUMBER_H     1
#define P_VERSION             2
#define P_ID                  3
#define P_BAUD_RATE           4
#define P_RETURN_DELAY_TIME   5
#define P_CW_ANGLE_LIMIT_L    6
#define P_CW_ANGLE_LIMIT_H    7
#define P_CCW_ANGLE_LIMIT_L   8
#define P_CCW_ANGLE_LIMIT_H   9
#define P_SYSTEM_DATA2        10
#define P_LIMIT_TEMPERATURE   11
#define P_DOWN_LIMIT_VOLTAGE  12
#define P_UP_LIMIT_VOLTAGE    13
#define P_MAX_TORQUE_L        14
#define P_MAX_TORQUE_H        15
#define P_RETURN_LEVEL        16
#define P_ALARM_LED           17
#define P_ALARM_SHUTDOWN      18
#define P_OPERATING_MODE      19
#define P_DOWN_CALIBRATION_L  20
#define P_DOWN_CALIBRATION_H  21
#define P_UP_CALIBRATION_L    22
#define P_UP_CALIBRATION_H    23

#define P_TORQUE_ENABLE         (24)
#define P_LED                   (25)

#define P_CW_COMPLIANCE_MARGIN  (26)
#define P_CCW_COMPLIANCE_MARGIN (27)
#define P_CW_COMPLIANCE_SLOPE   (28)
#define P_CCW_COMPLIANCE_SLOPE  (29)

#define P_P_GAIN  			(26)
#define P_I_GAIN 			(27)
#define P_D_GAIN   			(28)
#define P_RESERVED  		(29)


#define P_GOAL_POSITION_L       (30)
#define P_GOAL_POSITION_H       (31)
#define P_GOAL_SPEED_L          (32)
#define P_GOAL_SPEED_H          (33)
#define P_TORQUE_LIMIT_L        (34)
#define P_TORQUE_LIMIT_H        (35)
#define P_PRESENT_POSITION_L    (36)
#define P_PRESENT_POSITION_H    (37)
#define P_PRESENT_SPEED_L       (38)
#define P_PRESENT_SPEED_H       (39)
#define P_PRESENT_LOAD_L        (40)
#define P_PRESENT_LOAD_H        (41)
#define P_PRESENT_VOLTAGE       (42)
#define P_PRESENT_TEMPERATURE   (43)
#define P_REGISTERED_INSTRUCTION (44)
#define P_PAUSE_TIME            (45)
//#define P_FINAL_GOAL_POSITION_L (46)
#define P_MOVING (46)
#define P_FINAL_GOAL_POSITION_H (47)
#define P_MOVING_TIME_L         (48)
#define P_MOVING_TIME_H         (49)
#define P_FINAL_SPEED_L         (50)
#define P_FINAL_SPEED_H         (51)
#define P_POT_DATA_L  			(52)
#define P_POT_DATA_H  			(53)
#define P_PWM_OUT_L  			(54)
#define P_PWM_OUT_H  			(55)


/*
 * Index for packet instruction
 * */
#define ID					(2)
#define LENGTH				(3)
#define INSTRUCTION			(4)
#define ERRBIT				(4)
#define PARAMETER			(5)
#define DEFAULT_BAUDNUMBER	(1)

#define MAXNUM_RXPARAM		(60)
#define MAXNUM_TXPARAM		(150)


/*
 * defines for error message
 * */
#define ERRBIT_VOLTAGE		(1)
#define ERRBIT_ANGLE		(2)
#define ERRBIT_OVERHEAT		(4)
#define ERRBIT_RANGE		(8)
#define ERRBIT_CHECKSUM		(16)
#define ERRBIT_OVERLOAD		(32)
#define ERRBIT_INSTRUCTION	(64)


/*
 * defines message of communication
 * */
#define	COMM_TXSUCCESS		(0)
#define COMM_RXSUCCESS		(1)
#define COMM_TXFAIL			(2)
#define COMM_RXFAIL			(3)
#define COMM_TXERROR		(4)
#define COMM_RXWAITING		(5)
#define COMM_RXTIMEOUT		(6)
#define COMM_RXCORRUPT		(7)

/* timing defines */
#define RX_WAIT_TIMEOUT			0xf000
#define RX_TIMEOUT_COUNT2		(1600L) //(1000L) //porting
#define NANO_TIME_DELAY			(12000) //ydh added 20111228 -> 20120210 edited ydh
//#define RX_TIMEOUT_COUNT1  	(RX_TIMEOUT_COUNT2*90L)// -> 110msec  before Ver 1.11e
#define RX_TIMEOUT_COUNT1  		(RX_TIMEOUT_COUNT2*128L)//  -> 156msec for ID 110 safe access after Ver 1.11f //porting ydh
#define DOWNLOAD_TIMEOUT  		(RX_WAIT_TIMEOUT-0x1000)

/*Half Duflex UART ENABLE PIN ----------------------------------------------------------------------------------------------*/
#ifdef BOARD_CM900
#define PORT_ENABLE_TXD			GPIOB
#define PORT_ENABLE_RXD			GPIOB

#define PIN_ENABLE_TXD			4
#define PIN_ENABLE_RXD			5


//set to TXD Enable
#define	DXL_TXD	{ gpio_write_bit(PORT_ENABLE_TXD, PIN_ENABLE_TXD, 1 ); gpio_write_bit(PORT_ENABLE_RXD, PIN_ENABLE_RXD, 0 );}
//set to RXD Enable
#define	DXL_RXD	{ gpio_write_bit(PORT_ENABLE_TXD, PIN_ENABLE_TXD, 0 ); gpio_write_bit(PORT_ENABLE_RXD, PIN_ENABLE_RXD, 1 );}


#elif BOARD_CM900_REV10 || BOARD_CM904

#define PORT_TXRX_DIRECTION		GPIOB
#define PIN_TXRX_DIRECTION		5


#define	DXL_TXD	{ gpio_write_bit(PORT_TXRX_DIRECTION, PIN_TXRX_DIRECTION, 1); } //set to HIGH
#define	DXL_RXD	{ gpio_write_bit(PORT_TXRX_DIRECTION, PIN_TXRX_DIRECTION, 0); } //set to LOW


#endif

#define PORT_DXL_TXD			GPIOB
#define PORT_DXL_RXD			GPIOB

#define PIN_DXL_TXD				6
#define PIN_DXL_RXD				7

/*
 * For olny OpenCM9.04 485 SHD Board that uses USART3 as DXL3 Bus
 * */
#define PORT_TXRX_DIRECTION3	GPIOC
#define PIN_TXRX_DIRECTION3		15

#define PORT_DXL_TXD3			GPIOB
#define PORT_DXL_RXD3			GPIOB

#define PIN_DXL_TXD3			10
#define PIN_DXL_RXD3			11


#define	DXL_TXD3	{ gpio_write_bit(PORT_TXRX_DIRECTION3, PIN_TXRX_DIRECTION3, 1); } //set to HIGH
#define	DXL_RXD3	{ gpio_write_bit(PORT_TXRX_DIRECTION3, PIN_TXRX_DIRECTION3, 0); } //set to LOW


///////////////// utility for value ///////////////////////////
#define DXL_MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(((unsigned long)(a)) & 0xff)) | ((unsigned short)((unsigned char)(((unsigned long)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b)     ((unsigned int)(((unsigned short)(((unsigned long)(a)) & 0xffff)) | ((unsigned int)((unsigned short)(((unsigned long)(b)) & 0xffff))) << 16))
#define DXL_LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define DXL_HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define DXL_LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#define DXL_HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))



#endif /* DXL_CONSTANTS_H_ */
