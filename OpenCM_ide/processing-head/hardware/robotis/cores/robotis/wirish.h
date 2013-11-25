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
 * @brief Main include file for the Wirish core.
 *
 * Includes various Arduino wiring macros and bit defines
 */

#ifndef _WIRISH_H_
#define _WIRISH_H_

//#include "libpandora.h"

//#include "wirish_types.h"
#include "boards.h"
//#include "io.h"
#include "bits.h"
#include "pwm.h"
#include "ext_interrupts.h"
//#include "wirish_debug.h"
//#include "wirish_math.h"
//#include "wirish_time.h"
#include "HardwareSPI.h"
#include "HardwareSerial.h"
#include "HardwareTimer.h"
#include "usb_serial.h"

//[ROBOTIS]add to support dynamixel that is super powered robot actuator
#include "Dynamixel.h"
#include "Arduino-compatibles.h" //we need to decrease object files for shortening download time.

/* Arduino wiring macros and bit defines  */
#define HIGH 0x1
#define LOW  0x0

#define true 0x1
#define false 0x0

#define LSBFIRST 0
#define MSBFIRST 1

//#define lowByte(w)                     ((w) & 0xFF)  //can be replaced by DXL_LOBYTE()
//#define highByte(w)                    (((w) >> 8) & 0xFF) //can be replaced by DXL_LOBYTE()
#define bitRead(value, bit)            (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)             ((value) |= (1UL << (bit)))
#define bitClear(value, bit)           ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : \
                                                   bitClear(value, bit))
#define bit(b)                         (1UL << (b))

typedef uint8 boolean;
typedef uint8 byte;



/*************************************************************************************************************
 * Removed wirish_debug.h by ROBOTIS,.LTD. 2013-04-25
 * debug codes move to wirish.h
 * @brief High level debug port configuration
 *
 *************************************************************************************************************/

/**
 * @brief Disable the JTAG and Serial Wire (SW) debug ports.
 *
 * You can call this function in order to use the JTAG and SW debug
 * pins as ordinary GPIOs.
 *
 * @see enableDebugPorts()
 */
static inline void disableDebugPorts(void) {
    afio_cfg_debug_ports(AFIO_DEBUG_NONE);
}

/**
 * @brief Enable the JTAG and Serial Wire (SW) debug ports.
 *
 * After you call this function, the JTAG and SW debug pins will no
 * longer be usable as GPIOs.
 *
 * @see disableDebugPorts()
 */
static inline void enableDebugPorts(void) {
    afio_cfg_debug_ports(AFIO_DEBUG_FULL_SWJ);
}

#endif

