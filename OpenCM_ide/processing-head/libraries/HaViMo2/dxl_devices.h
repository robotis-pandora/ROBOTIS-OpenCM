/*
 *******************************************************************************
 *  dxl_devices.h - 2013-04-05-1300
 *******************************************************************************
 *  A header file of questionable quality containing the address tables of
 *    several Dynamixel devices.  It should work with the AX-12/18, AX-S1,
 *    AX-S20, HaViMo2, Jon Hylands' FootPressureSensor kit, MX-28, IR Sensor 
 *    array, and CM-730.
 *  
 *******************************************************************************
 *  LEGAL STUFF
 *******************************************************************************
 *  
 *   'Dynamixel' is property of Robotis, Inc.
 *      http://www.robotis.com
 *  
 *  
 *  Copyright (c) 2011, 2012, 2013 Matthew Paulishen. All rights reserved.
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *******************************************************************************
 */
 
#ifndef _DXL_DEVS_H_
#define _DXL_DEVS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


// Default model numbers
#define MODEL_AX12                      12
#define MODEL_AX12W                     300
#define MODEL_AX18                      18
#define MODEL_AXS1                      13
#define MODEL_AXS20                     212
#define MODEL_JHFPS                     17163	// 0x6711
#define MODEL_HaViMo2                   0
//#define MODEL_CM730                     ?
#define MODEL_MX28                      29
#define MODEL_IRSA                      330     // 0x014A



// Universal address table
#define P_MODEL_NUMBER_L                0
#define P_MODEL_NUMBER_H                1
#define P_FIRMWARE_VERSION              2
#define P_ID                            3
#define P_BAUD_RATE                     4
#define P_RETURN_DELAY_TIME             5
#define P_STATUS_RETURN_LEVEL           16
//#define P_LED                           25  // AX-12+, AX-18F, and AX-S20
//#define P_REGISTERED_INSTRUCTION        44



// Robotis' IR Sensor Array
enum{
    IRSA_MODEL_NUMBER_L               = 0,
    IRSA_MODEL_NUMBER_H               = 1,
    IRSA_FIRMWARE_VERSION             = 2,
    IRSA_ID                           = 3,
    IRSA_BAUD_RATE                    = 4,
    IRSA_RETURN_DELAY_TIME            = 5,
    IRSA_ETH_L_1                      = 6,
    IRSA_ETH_H_1                      = 7,
    IRSA_ETH_L_2                      = 8,
    IRSA_ETH_H_2                      = 9,
    IRSA_ETH_L_3                      = 10,
    IRSA_ETH_H_3                      = 11,
    IRSA_ETH_L_4                      = 12,
    IRSA_ETH_H_4                      = 13,
    IRSA_ETH_L_5                      = 14,
    IRSA_ETH_H_5                      = 15,
    IRSA_STATUS_RETURN_LEVEL          = 16,
    IRSA_ETH_L_6                      = 17,
    IRSA_ETH_H_6                      = 18,
    IRSA_ETH_L_7                      = 20,
    IRSA_ETH_H_7                      = 21,
    IRSA_FD_L_1                       = 24,
    IRSA_FD_H_1                       = 25,
    IRSA_FD_L_2                       = 26,
    IRSA_FD_H_2                       = 27,
    IRSA_FD_L_3                       = 28,
    IRSA_FD_H_3                       = 29,
    IRSA_FD_L_4                       = 30,
    IRSA_FD_H_4                       = 31,
    IRSA_FD_L_5                       = 32,
    IRSA_FD_H_5                       = 33,
    IRSA_FD_L_6                       = 34,
    IRSA_FD_H_6                       = 35,
    IRSA_FD_L_7                       = 36,
    IRSA_FD_H_7                       = 37,
    IRSA_BUZZ_NOTE                    = 40,
    IRSA_BUZZ_TIME                    = 41,
    IRSA_AUTO_TH                      = 42,
    IRSA_OBS_DET                      = 43,
    IRSA_REGISTERED_INSTRUCTION       = 44,
    IRSA_LOCK                         = 47,
    IRSA_TH_L_1                       = 48,
    IRSA_TH_H_1                       = 49,
    IRSA_TH_L_2                       = 50,
    IRSA_TH_H_2                       = 51,
    IRSA_TH_L_3                       = 52,
    IRSA_TH_H_3                       = 53,
    IRSA_TH_L_4                       = 54,
    IRSA_TH_H_4                       = 55,
    IRSA_TH_L_5                       = 56,
    IRSA_TH_H_5                       = 57,
    IRSA_TH_L_6                       = 58,
    IRSA_TH_H_6                       = 59,
    IRSA_TH_L_7                       = 60,
    IRSA_TH_H_7                       = 61
};

// John Hyland's FSR FOOT PRESSURE SENSOR BOARD
enum{
    JHFPS_MODEL_NUMBER_L              = 0,
    JHFPS_MODEL_NUMBER_H              = 1,
    JHFPS_FIRMWARE_VERSION            = 2,
    JHFPS_ID                          = 3,
    JHFPS_BAUD_RATE                   = 4,
    JHFPS_RETURN_DELAY_TIME           = 5,
    JHFPS_STATUS_RETURN_LEVEL         = 16,
    JHFPS_LED                         = 25,
    JHFPS_FRONT_INSIDE_L              = 26,
    JHFPS_FRONT_INSIDE_H              = 27,
    JHFPS_FRONT_OUTSIDE_L             = 28,
    JHFPS_FRONT_OUTSIDE_H             = 29,
    JHFPS_REAR_INSIDE_L               = 30,
    JHFPS_REAR_INSIDE_H               = 31,
    JHFPS_REAR_OUTSIDE_L              = 32,
    JHFPS_REAR_OUTSIDE_H              = 33
};

//#include <HaViMo2.h>

// AX-12+/18F address table
enum{
    AXM_MODEL_NUMBER_L                = 0,
    AXM_MODEL_NUMBER_H                = 1,
    AXM_FIRMWARE_VERSION              = 2,
    AXM_ID                            = 3,
    AXM_BAUD_RATE                     = 4,
    AXM_RETURN_DELAY_TIME             = 5,
    AXM_CW_ANGLE_LIMIT_L              = 6,
    AXM_CW_ANGLE_LIMIT_H              = 7,
    AXM_CCW_ANGLE_LIMIT_L             = 8,
    AXM_CCW_ANGLE_LIMIT_H             = 9,
    AXM_SYSTEM_DATA2                  = 10,
    AXM_HIGHEST_LIMIT_TEMPERATURE     = 11,
    AXM_LOWEST_LIMIT_VOLTAGE          = 12,
    AXM_HIGHEST_LIMIT_VOLTAGE         = 13,
    AXM_MAX_TORQUE_L                  = 14,
    AXM_MAX_TORQUE_H                  = 15,
    AXM_STATUS_RETURN_LEVEL           = 16,
    AXM_ALARM_LED                     = 17,
    AXM_ALARM_SHUTDOWN                = 18,
    AXM_OPERATING_MODE                = 19,
    AXM_DOWN_CALIBRATION_L            = 20,
    AXM_DOWN_CALIBRATION_H            = 21,
    AXM_UP_CALIBRATION_L              = 22,
    AXM_UP_CALIBRATION_H              = 23,
    AXM_TORQUE_ENABLE                 = 24,
    AXM_LED                           = 25,
    AXM_CW_COMPLIANCE_MARGIN          = 26,
    AXM_CCW_COMPLIANCE_MARGIN         = 27,
    AXM_CW_COMPLIANCE_SLOPE           = 28,
    AXM_CCW_COMPLIANCE_SLOPE          = 29,
    AXM_GOAL_POSITION_L               = 30,
    AXM_GOAL_POSITION_H               = 31,
    AXM_MOVING_SPEED_L                = 32,
    AXM_MOVING_SPEED_H                = 33,
    AXM_TORQUE_LIMIT_L                = 34,
    AXM_TORQUE_LIMIT_H                = 35,
    AXM_PRESENT_POSITION_L            = 36,
    AXM_PRESENT_POSITION_H            = 37,
    AXM_PRESENT_SPEED_L               = 38,
    AXM_PRESENT_SPEED_H               = 39,
    AXM_PRESENT_LOAD_L                = 40,
    AXM_PRESENT_LOAD_H                = 41,
    AXM_PRESENT_VOLTAGE               = 42,
    AXM_PRESENT_TEMPERATURE           = 43,
    AXM_REGISTERED_INSTRUCTION        = 44,
    AXM_PAUSE_TIME                    = 45,
    AXM_MOVING                        = 46,
    AXM_LOCK                          = 47,
    AXM_PUNCH_L                       = 48,
    AXM_PUNCH_H                       = 49
};


// AX-S1 Address table
enum{
    AXS1_MODEL_NUMBER_L               = 0,
    AXS1_MODEL_NUMBER_H               = 1,
    AXS1_FIRMWARE_VERSION             = 2,
    AXS1_ID                           = 3,
    AXS1_BAUD_RATE                    = 4,
    AXS1_RETURN_DELAY_TIME            = 5,
    AXS1_STATUS_RETURN_LEVEL          = 16,
    AXS1_IR_LEFT                      = 26,
    AXS1_IR_CENTER                    = 27,
    AXS1_IR_RIGHT                     = 28,
    AXS1_LUMINOSITY_LEFT              = 29,
    AXS1_LUMINOSITY_CENTER            = 30,
    AXS1_LUMINOSITY_RIGHT             = 31,
    AXS1_IR_OBSTACLE_DETECTED         = 32,
    AXS1_LUMINOSITY_DETECTED          = 33,
    AXS1_SOUND_DATA                   = 35,
    AXS1_SOUND_DATA_MAX_HOLD          = 36,
    AXS1_SOUND_DETECT_COUNT           = 37,
    AXS1_SOUND_DETECT_TIME_L          = 38,
    AXS1_SOUND_DETECT_TIME_H          = 39,
    AXS1_BUZZER_INDEX                 = 40,
    AXS1_BUZZER_TIME                  = 41,
    AXS1_IR_REMOCON_ARRIVED           = 46,
    AXS1_IR_REMOCON_RX_DATA_L         = 48,
    AXS1_IR_REMOCON_RX_DATA_H         = 49,
    AXS1_IR_REMOCON_TX_DATA_L         = 50,
    AXS1_IR_REMOCON_TX_DATA_H         = 51,
    AXS1_OBSTACLE_DETECTED_COMPARE    = 52,
    AXS1_LIGHT_DETECTED_COMPARE       = 53
};


// AX-S20 address table
enum{
    AXS20_MODEL_NUMBER_L              = 0,
    AXS20_MODEL_NUMBER_H              = 1,
    AXS20_FIRMWARE_VERSION            = 2,
    AXS20_ID                          = 3,
    AXS20_BAUD_RATE                   = 4,
    AXS20_RETURN_DELAY_TIME           = 5,
    AXS20_STATUS_RETURN_LEVEL         = 16,
    AXS20_LED                         = 25, // AX-12+, AX-18F, and AX-S20
    AXS20_AZIMUTH_Z_L                 = 26, // 0-360 degrees            +CW from top/-CCW from top
    AXS20_AZIMUTH_Z_H                 = 27,
    AXS20_PITCH_Y_L                   = 28, // -90-90 degrees [word]    +forward/-backward
    AXS20_PITCH_Y_H                   = 29,
    AXS20_ROLL_X_L                    = 30, // -90-90 degrees [word]    +left/-right
    AXS20_ROLL_X_H                    = 31,
    AXS20_MAGNETO_Z_L                 = 32, // raw magneto flux density
    AXS20_MAGNETO_Z_H                 = 33,
    AXS20_MAGNETO_X_L                 = 34, // raw magneto flux density
    AXS20_MAGNETO_X_H                 = 35,
    AXS20_MAGNETO_Y_L                 = 36, // raw magneto flux density
    AXS20_MAGNETO_Y_H                 = 37,
    AXS20_ACCEL_X_L                   = 38, // -2048~2048               +forward/-backward
    AXS20_ACCEL_X_H                   = 39,
    AXS20_ACCEL_Y_L                   = 40, // -2048~2048               +left/-right
    AXS20_ACCEL_Y_H                   = 41,
    AXS20_ACCEL_Z_L                   = 46, // -2048~2048               +up/-down
    AXS20_ACCEL_Z_H                   = 47,
    AXS20_ACCEL_PITCH_Y               = 48, // -90~90 degrees [byte] from accelerometer
    AXS20_ACCEL_ROLL_X                = 49  // -90~90 degrees [byte] from accelerometer

};


// MX-28 address table
#ifdef MX28_1024
enum{
    MX_MODEL_NUMBER_L                 = 0,
    MX28_MODEL_NUMBER_H               = 1,
    MX28_VERSION                      = 2,
    MX28_ID                           = 3,
    MX28_BAUD_RATE                    = 4,
    MX28_RETURN_DELAY_TIME            = 5,
    MX28_CW_ANGLE_LIMIT_L             = 6,
    MX28_CW_ANGLE_LIMIT_H             = 7,
    MX28_CCW_ANGLE_LIMIT_L            = 8,
    MX28_CCW_ANGLE_LIMIT_H            = 9,
    MX28_HIGH_LIMIT_TEMPERATURE       = 11,
    MX28_LOW_LIMIT_VOLTAGE            = 12,
    MX28_HIGH_LIMIT_VOLTAGE           = 13,
    MX28_MAX_TORQUE_L                 = 14,
    MX28_MAX_TORQUE_H                 = 15,
    MX28_RETURN_LEVEL                 = 16,
    MX28_ALARM_LED                    = 17,
    MX28_ALARM_SHUTDOWN               = 18,
    MX28_TORQUE_ENABLE                = 24,
    MX28_LED                          = 25,
    MX28_CW_COMPLIANCE_MARGIN         = 26,
    MX28_CCW_COMPLIANCE_MARGIN        = 27,
    MX28_CW_COMPLIANCE_SLOPE          = 28,
    MX28_CCW_COMPLIANCE_SLOPE         = 29,
    MX28_GOAL_POSITION_L              = 30,
    MX28_GOAL_POSITION_H              = 31,
    MX28_MOVING_SPEED_L               = 32,
    MX28_MOVING_SPEED_H               = 33,
    MX28_TORQUE_LIMIT_L               = 34,
    MX28_TORQUE_LIMIT_H               = 35,
    MX28_PRESENT_POSITION_L           = 36,
    MX28_PRESENT_POSITION_H           = 37,
    MX28_PRESENT_SPEED_L              = 38,
    MX28_PRESENT_SPEED_H              = 39,
    MX28_PRESENT_LOAD_L               = 40,
    MX28_PRESENT_LOAD_H               = 41,
    MX28_PRESENT_VOLTAGE              = 42,
    MX28_PRESENT_TEMPERATURE          = 43,
    MX28_REGISTERED_INSTRUCTION       = 44,
    MX28_MOVING                       = 46,
    MX28_LOCK                         = 47,
    MX28_PUNCH_L                      = 48,
    MX28_PUNCH_H                      = 49
};
#else
enum{
    MX28_MODEL_NUMBER_L               = 0,
    MX28_MODEL_NUMBER_H               = 1,
    MX28_VERSION                      = 2,
    MX28_ID                           = 3,
    MX28_BAUD_RATE                    = 4,
    MX28_RETURN_DELAY_TIME            = 5,
    MX28_CW_ANGLE_LIMIT_L             = 6,
    MX28_CW_ANGLE_LIMIT_H             = 7,
    MX28_CCW_ANGLE_LIMIT_L            = 8,
    MX28_CCW_ANGLE_LIMIT_H            = 9,
    MX28_SYSTEM_DATA2                 = 10,
    MX28_HIGH_LIMIT_TEMPERATURE       = 11,
    MX28_LOW_LIMIT_VOLTAGE            = 12,
    MX28_HIGH_LIMIT_VOLTAGE           = 13,
    MX28_MAX_TORQUE_L                 = 14,
    MX28_MAX_TORQUE_H                 = 15,
    MX28_RETURN_LEVEL                 = 16,
    MX28_ALARM_LED                    = 17,
    MX28_ALARM_SHUTDOWN               = 18,
    MX28_OPERATING_MODE               = 19,
    MX28_LOW_CALIBRATION_L            = 20,
    MX28_LOW_CALIBRATION_H            = 21,
    MX28_HIGH_CALIBRATION_L           = 22,
    MX28_HIGH_CALIBRATION_H           = 23,
    MX28_TORQUE_ENABLE                = 24,
    MX28_LED                          = 25,
    MX28_P_GAIN                       = 26,
    MX28_I_GAIN                       = 27,
    MX28_D_GAIN                       = 28,
    MX28_RESERVED                     = 29,
    MX28_GOAL_POSITION_L              = 30,
    MX28_GOAL_POSITION_H              = 31,
    MX28_MOVING_SPEED_L               = 32,
    MX28_MOVING_SPEED_H               = 33,
    MX28_TORQUE_LIMIT_L               = 34,
    MX28_TORQUE_LIMIT_H               = 35,
    MX28_PRESENT_POSITION_L           = 36,
    MX28_PRESENT_POSITION_H           = 37,
    MX28_PRESENT_SPEED_L              = 38,
    MX28_PRESENT_SPEED_H              = 39,
    MX28_PRESENT_LOAD_L               = 40,
    MX28_PRESENT_LOAD_H               = 41,
    MX28_PRESENT_VOLTAGE              = 42,
    MX28_PRESENT_TEMPERATURE          = 43,
    MX28_REGISTERED_INSTRUCTION       = 44,
    MX28_PAUSE_TIME                   = 45,
    MX28_MOVING                       = 46,
    MX28_LOCK                         = 47,
    MX28_PUNCH_L                      = 48,
    MX28_PUNCH_H                      = 49,
    MX28_RESERVED4                    = 50,
    MX28_RESERVED5                    = 51,
    MX28_POT_L                        = 52,
    MX28_POT_H                        = 53,
    MX28_PWM_OUT_L                    = 54,
    MX28_PWM_OUT_H                    = 55,
    MX28_P_ERROR_L                    = 56,
    MX28_P_ERROR_H                    = 57,
    MX28_I_ERROR_L                    = 58,
    MX28_I_ERROR_H                    = 59,
    MX28_D_ERROR_L                    = 60,
    MX28_D_ERROR_H                    = 61,
    MX28_P_ERROR_OUT_L                = 62,
    MX28_P_ERROR_OUT_H                = 63,
    MX28_I_ERROR_OUT_L                = 64,
    MX28_I_ERROR_OUT_H                = 65,
    MX28_D_ERROR_OUT_L                = 66,
    MX28_D_ERROR_OUT_H                = 67
};
#endif


// CM-730 address table
enum{
    CM730_MODEL_NUMBER_L              = 0,
    CM730_MODEL_NUMBER_H              = 1,
    CM730_FIRMWARE_VERSION            = 2,
    CM730_ID                          = 3,
    CM730_BAUD_RATE                   = 4,
    CM730_RETURN_DELAY_TIME           = 5,
    CM730_STATUS_RETURN_LEVEL         = 16,
    CM730_DXL_POWER                   = 24,
    CM730_LED_PANEL                   = 25,
    CM730_LED_HEAD_L                  = 26,
    CM730_LED_HEAD_H                  = 27,
    CM730_LED_EYE_L                   = 28,
    CM730_LED_EYE_H                   = 29,
    CM730_BUTTON                      = 30,
    CM730_GYRO_Z_L                    = 38,
    CM730_GYRO_Z_H                    = 39,
    CM730_GYRO_Y_L                    = 40,
    CM730_GYRO_Y_H                    = 41,
    CM730_GYRO_X_L                    = 42,
    CM730_GYRO_X_H                    = 43,
    CM730_ACCEL_X_L                   = 44,
    CM730_ACCEL_X_H                   = 45,
    CM730_ACCEL_Y_L                   = 46,
    CM730_ACCEL_Y_H                   = 47,
    CM730_ACCEL_Z_L                   = 48,
    CM730_ACCEL_Z_H                   = 49,
    CM730_VOLTAGE                     = 50,
    CM730_LEFT_MIC_L                  = 51,
    CM730_LEFT_MIC_H                  = 52,
    CM730_ADC2_L                      = 53,
    CM730_ADC2_H                      = 54,
    CM730_ADC3_L                      = 55,
    CM730_ADC3_H                      = 56,
    CM730_ADC4_L                      = 57,
    CM730_ADC4_H                      = 58,
    CM730_ADC5_L                      = 59,
    CM730_ADC5_H                      = 60,
    CM730_ADC6_L                      = 61,
    CM730_ADC6_H                      = 62,
    CM730_ADC7_L                      = 63,
    CM730_ADC7_H                      = 64,
    CM730_ADC8_L                      = 65,
    CM730_ADC8_H                      = 66,
    CM730_RIGHT_MIC_L                 = 67,
    CM730_RIGHT_MIC_H                 = 68,
    CM730_ADC10_L                     = 69,
    CM730_ADC10_H                     = 70,
    CM730_ADC11_L                     = 71,
    CM730_ADC11_H                     = 72,
    CM730_ADC12_L                     = 73,
    CM730_ADC12_H                     = 74,
    CM730_ADC13_L                     = 75,
    CM730_ADC13_H                     = 76,
    CM730_ADC14_L                     = 77,
    CM730_ADC14_H                     = 78,
    CM730_ADC15_L                     = 79,
    CM730_ADC15_H                     = 80
};

#ifdef __cplusplus
}
#endif



#endif
