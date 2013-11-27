/*
 * cm900.cpp
 *
 *  Created on: 2012. 10. 14.
 *      Author: in2storm
 */




/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs, LLC.
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
 * @file   maple.cpp
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief  Maple PIN_MAP and boardInit().
 */

#if defined(BOARD_CM900) || defined(BOARD_CM900_REV10)
#include "boards.h"

#include "gpio.h"
#include "timer.h"


void boardInit(void) {
	afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY); //[ROBOTIS] 2013-07-17
	//[ROBOTIS][CHANGE] add here if you want to initialize something
	gpio_set_mode(GPIOB, 2, GPIO_OUTPUT_PP);
	gpio_write_bit(GPIOB, 2,1); //LED off when start board
}

extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS] = {

    /* Top header */
	{GPIOA, TIMER2, ADC1,  0, 1,    0}, /* D0/PA0 */
	{GPIOA, TIMER2, ADC1,  1, 2,    1}, /* D1/PA1 */
	{GPIOA, TIMER2, ADC1,  2, 3,    2}, /* D2/PA2 */
	{GPIOA, TIMER2, ADC1,  3, 4,    3}, /* D3/PA3 */
	{GPIOA,   NULL, ADC1,  4, 0,    4}, /* D4/PA4 */
	{GPIOA,   NULL, ADC1,  5, 0,    5}, /* D5/PA5 */
	{GPIOA, TIMER3, ADC1,  6, 1,    6}, /* D6/PA6 */
	{GPIOA, TIMER3, ADC1,  7, 2,    7}, /* D7/PA7 */
	{GPIOA, TIMER1, NULL,  8, 1, ADCx}, /* D8/PA8 */
	{GPIOA, TIMER1, NULL,  9, 2, ADCx}, /* D9/PA9 */

	{GPIOA, TIMER1, NULL, 10, 3, ADCx}, /* D10/PA10 */
	{GPIOA,   NULL, NULL, 13, 0, ADCx}, /* D11/PA13 */
	{GPIOA,   NULL, NULL, 14, 0, ADCx}, /* D12/PA14 */
	{GPIOA,   NULL, NULL, 15, 0, ADCx}, /* D13/PA15 */
	{GPIOB, TIMER3, ADC1,  0, 3,    8}, /* D14/PB0 */
	{GPIOB, TIMER3, ADC1,  1, 4,    9}, /* D15/PB1 */
	{GPIOB,   NULL, NULL,  2, 0, ADCx}, /* D16/PB2 (LED)*/
	{GPIOB,   NULL, NULL,  3, 0, ADCx}, /* D17/PB3  */
	{GPIOB,   NULL, NULL,  4, 0, ADCx}, /* D18/PB4  */
	{GPIOB,   NULL, NULL,  5, 0, ADCx}, /* D19/PB5 */

	{GPIOB, TIMER4, NULL,  6, 1, ADCx}, /* D20/PB6 */
	{GPIOB, TIMER4, NULL,  7, 2, ADCx}, /* D21/PB7 */
	{GPIOB, TIMER4, NULL,  8, 3, ADCx}, /* D22/PB8 */
	{GPIOB, TIMER4, NULL,  9, 4, ADCx}, /* D23/PB9 */
	{GPIOB,   NULL, NULL, 10, 0, ADCx}, /* D24/PB10 */
	{GPIOB,   NULL, NULL, 11, 0, ADCx}, /* D25/PB11 */
	{GPIOB,   NULL, NULL, 12, 0, ADCx}, /* D26/PB12 */
	{GPIOB,   NULL, NULL, 13, 0, ADCx}, /* D27/PB13 */
	{GPIOB,   NULL, NULL, 14, 0, ADCx}, /* D28/PB14 */
	{GPIOB,   NULL, NULL, 15, 0, ADCx}, /* D29/PB15 */

	//{GPIOC,   NULL, NULL, 13, 0, ADCx}, /* --/PC13 */
	{GPIOC,   NULL, NULL, 14, 0, ADCx}, /* D30/PC14 */
	{GPIOC,   NULL, NULL, 15, 0, ADCx} /* D31/PC15 */


    /* JTAG header */

    //{GPIOA,   NULL, NULL, 13, 0, ADCx}, /* D13/PA13 */
    //{GPIOA,   NULL, NULL, 14, 0, ADCx}, /* D14/PA14 */
    //{GPIOA,   NULL, NULL, 15, 0, ADCx}, /* D15/PA15 */
    //{GPIOB,   NULL, NULL,  3, 0, ADCx}, /* D17/PB3  */
    //{GPIOB,   NULL, NULL,  4, 0, ADCx}, /* D18/PB4  */
};
/**
 * [ROBOTIS][CHANGE] 2013-04-22
 * Don't need to use the below variables, I think it is not versatile variable.
 * */
/*extern const uint8 boardPWMPins[] __FLASH__ = {
    0, 1, 2, 3, 5, 6, 7, 8, 9, 11, 12, 14, 24, 27, 28
};

extern const uint8 boardADCPins[] __FLASH__ = {
    0, 1, 2, 3, 10, 11, 12, 15, 16, 17, 18, 19, 20, 27, 28
};

extern const uint8 boardUsedPins[] __FLASH__ = {
    BOARD_LED_PIN, BOARD_BUTTON_PIN, BOARD_JTMS_SWDIO_PIN,
    BOARD_JTCK_SWCLK_PIN, BOARD_JTDI_PIN, BOARD_JTDO_PIN, BOARD_NJTRST_PIN
};*/

#endif
