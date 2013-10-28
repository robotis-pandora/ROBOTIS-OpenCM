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
 *  @file libpandora_types.h
 * [ROBOTIS] change file name from libmaple_types.h to libpandora_types.h
 *  @brief libmaple types
 */

#ifndef _LIBMAPLE_TYPES_H_
#define _LIBMAPLE_TYPES_H_

#define NVIC_BASE                       ((struct nvic_reg_map*)0xE000E100)

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

/**
 * [ROBOTIS][START] support for Dynamixel SDK.
 * */
#define word			uint16
#define byte			uint8
#define dword			uint32
/**
 * [ROBOTIS][END] support for Dynamixel SDK.
 * */

/*
 * [ROBOTIS][START]2012-12-13 to support user interrupt func.
 * */

typedef void (*voidFuncPtrUart)(byte);
typedef void (*voidFuncPtrUsb)(byte*, byte );

/*
 * [ROBOTIS][END]2012-12-13 to support user interrupt func.
 * */

typedef void (*voidFuncPtr)(void);

#define __io volatile
#define __attr_flash __attribute__((section (".USER_FLASH")))

#ifndef NULL
#define NULL 0
#endif



#endif

