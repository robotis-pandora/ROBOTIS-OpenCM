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
 * @file usart.c
 * @author Marti Bolivar <mbolivar@leaflabs.com>,
 *         Perry Hung <perry@leaflabs.com>
 * @brief USART control routines
 */

#include "usart.h"

//[START] ASSERT(exp) for  DEBUG
#include "gpio.h" //debug assert()
#include "adc.h"
#include "timer.h"
#include "delay.h"
//[END] ASSERT(exp) for  DEBUG

/*
 * [ROBOTIS]2012-12-13 to support user interrupt for USART devices
 *
 * */
voidFuncPtrUart userUsartInterrupt1 = NULL;
voidFuncPtrUart userUsartInterrupt2 = NULL;
voidFuncPtrUart userUsartInterrupt3 = NULL;
#ifdef STM32_HIGH_DENSITY
voidFuncPtrUart userUsartInterrupt4 = NULL;
voidFuncPtrUart userUsartInterrupt5 = NULL;
#endif



/*
 * Devices
 */


static ring_buffer usart1_rb;
static usart_dev usart1 = {
    .regs     = USART1_BASE,
    .rb       = &usart1_rb,
    .max_baud = 4500000UL,
    .clk_id   = RCC_USART1,
    .irq_num  = NVIC_USART1
};
/** USART1 device */
usart_dev *USART1 = &usart1;

static ring_buffer usart2_rb;
static usart_dev usart2 = {
    .regs     = USART2_BASE,
    .rb       = &usart2_rb,
    .max_baud = 2250000UL,
    .clk_id   = RCC_USART2,
    .irq_num  = NVIC_USART2
};
/** USART2 device */
usart_dev *USART2 = &usart2;

static ring_buffer usart3_rb;
static usart_dev usart3 = {
    .regs     = USART3_BASE,
    .rb       = &usart3_rb,
    .max_baud = 2250000UL,
    .clk_id   = RCC_USART3,
    .irq_num  = NVIC_USART3
};
/** USART3 device */
usart_dev *USART3 = &usart3;

#ifdef STM32_HIGH_DENSITY
static ring_buffer uart4_rb;
static usart_dev uart4 = {
    .regs     = UART4_BASE,
    .rb       = &uart4_rb,
    .max_baud = 2250000UL,
    .clk_id   = RCC_UART4,
    .irq_num  = NVIC_UART4
};
/** UART4 device */
usart_dev *UART4 = &uart4;

static ring_buffer uart5_rb;
static usart_dev uart5 = {
    .regs     = UART5_BASE,
    .rb       = &uart5_rb,
    .max_baud = 2250000UL,
    .clk_id   = RCC_UART5,
    .irq_num  = NVIC_UART5
};
/** UART5 device */
usart_dev *UART5 = &uart5;
#endif

/**
 * @brief Initialize a serial port.
 * @param dev         Serial port to be initialized
 */
void usart_init(usart_dev *dev) {
    rb_init(dev->rb, USART_RX_BUF_SIZE, dev->rx_buf);
    rcc_clk_enable(dev->clk_id);
    nvic_irq_enable(dev->irq_num);
}

/**
 * @brief Configure a serial port's baud rate.
 *
 * @param dev         Serial port to be configured
 * @param clock_speed Clock speed, in megahertz.
 * @param baud        Baud rate for transmit/receive.
 */
void usart_set_baud_rate(usart_dev *dev, uint32 clock_speed, uint32 baud) {
    uint32 integer_part;
    uint32 fractional_part;
    uint32 tmp;

    /* See ST RM0008 for the details on configuring the baud rate register */
    integer_part = (25 * clock_speed) / (4 * baud);
    tmp = (integer_part / 100) << 4;
    fractional_part = integer_part - (100 * (tmp >> 4));
    tmp |= (((fractional_part * 16) + 50) / 100) & ((uint8)0x0F);

    //TxDStringC("tmp clock = ");TxDHex16C(tmp);TxDStringC("\r\n");
    dev->regs->BRR = (uint16)tmp;
}

/**
 * @brief Enable a serial port.
 *
 * USART is enabled in single buffer transmission mode, multibuffer
 * receiver mode, 8n1.
 *
 * Serial port must have a baud rate configured to work properly.
 *
 * @param dev Serial port to enable.
 * @see usart_set_baud_rate()
 */
void usart_enable(usart_dev *dev) {
    usart_reg_map *regs = dev->regs;
    regs->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
    regs->CR1 |= USART_CR1_UE;
}

/**
 * @brief Turn off a serial port.
 * @param dev Serial port to be disabled
 */
void usart_disable(usart_dev *dev) {
    /* FIXME this misbehaves if you try to use PWM on TX afterwards */
    usart_reg_map *regs = dev->regs;

    /* TC bit must be high before disabling the USART */
    while((regs->CR1 & USART_CR1_UE) && !(regs->SR & USART_SR_TC))
        ;

    /* Disable UE */
    regs->CR1 &= ~USART_CR1_UE;

    /* Clean up buffer */
    usart_reset_rx(dev);
}

/**
 *  @brief Call a function on each USART.
 *  @param fn Function to call.
 */
void usart_foreach(void (*fn)(usart_dev*)) {
    fn(USART1);
    fn(USART2);
    fn(USART3);
#ifdef STM32_HIGH_DENSITY
    fn(UART4);
    fn(UART5);
#endif
}

/**
 * @brief Nonblocking USART transmit
 * @param dev Serial port to transmit over
 * @param buf Buffer to transmit
 * @param len Maximum number of bytes to transmit
 * @return Number of bytes transmitted
 */
uint32 usart_tx(usart_dev *dev, const uint8 *buf, uint32 len) {

	//TxDHex8C(buf[0]);
    usart_reg_map *regs = dev->regs;
    uint32 txed = 0;
    while ((regs->SR & USART_SR_TXE) && (txed < len)) {
        regs->DR = buf[txed++];
    }
    return txed;
}

/**
 * @brief Transmit an unsigned integer to the specified serial port in
 *        decimal format.
 *
 * This function blocks until the integer's digits have been
 * completely transmitted.
 *
 * @param dev Serial port to send on
 * @param val Number to print
 */
void usart_putudec(usart_dev *dev, uint32 val) {
    char digits[12];
    int i = 0;

    do {
        digits[i++] = val % 10 + '0';
        val /= 10;
    } while (val > 0);

    while (--i >= 0) {
        usart_putc(dev, digits[i]);
    }
}



/**[ROBOTIS][START]2012-12-13 to support user interrupt API
 * @brief Attach a USART interrupt.
 * @param dev USART device
 * @param interrupt Interrupt number to attach to USART DeviceX
 * @param handler Handler to attach to the given interrupt.
 *
 */
void usart_attach_interrupt(usart_dev *dev,
                            voidFuncPtrUart handler) {

	if ( dev == USART1){
		userUsartInterrupt1 = handler;
	}
	else if(dev == USART2){
		userUsartInterrupt2 = handler;
	}
	else if(dev == USART3){
		userUsartInterrupt3 = handler;
	}
#ifdef STM32_HIGH_DENSITY
	else if(dev == USART4){
		userUsartInterrupt4 = handler;
	}
	else if(dev == USART5){
		userUsartInterrupt5 = handler;
	}
#endif
}
/*
 * @brief detach a USART interrupt.
 * @param dev USART device
 * */
void usart_detach_interrupt(usart_dev *dev){
	if ( dev == USART1){
		userUsartInterrupt1 = NULL;
	}
	else if(dev == USART2){
		userUsartInterrupt2 = NULL;
	}
	else if(dev == USART3){
		userUsartInterrupt3 = NULL;
	}
#ifdef STM32_HIGH_DENSITY
	else if(dev == USART4){
		userUsartInterrupt4 = NULL;
	}
	else if(dev == USART5){
		userUsartInterrupt5 = NULL;
	}
#endif

}
/*
 *[ROBOTIS][START]2012-12-13 to support user interrupt API
 *
 * */



/*
 * Interrupt handlers.
 */
#include "usb_type.h"

static inline void usart_irq(usart_dev *dev) {
#ifdef USART_SAFE_INSERT
    /* If the buffer is full and the user defines USART_SAFE_INSERT,
     * ignore new bytes. */
    rb_safe_insert(dev->rb, (uint8)dev->regs->DR);
#else
    /* By default, push bytes around in the ring buffer. */

    rb_push_insert(dev->rb, (uint8)dev->regs->DR);
#endif
}

void __irq_usart1(void) {

	if ((USART1->regs->SR & USART_SR_RXNE) != (u16)RESET){
		if(userUsartInterrupt1 != NULL){ // user interrupt
			userUsartInterrupt1((byte)USART1->regs->DR);
			return;
		}
		usart_irq(USART1);
	}


}


void __irq_usart2(void) {

	if ((USART2->regs->SR & USART_SR_RXNE) != (u16)RESET){

		if(userUsartInterrupt2 != NULL){
				userUsartInterrupt2((byte)USART2->regs->DR);
				return;
			}


		usart_irq(USART2);
	}

}
extern volatile byte  gbDXLWritePointer3;
extern volatile byte  gbpDXLDataBuffer3[256];
extern uint8 gbIsDynmixelUsed3; //[ROBOTIS]2012-12-13

void __irq_usart3(void) {

	if ((USART3->regs->SR & USART_SR_RXNE) != (u16)RESET){
		if(userUsartInterrupt3 != NULL){
			userUsartInterrupt3((byte)USART3->regs->DR);
			return;
		}
		usart_irq(USART3);
	}

}

#ifdef STM32_HIGH_DENSITY
void __irq_uart4(void) {
	if ((USART4->regs->SR & USART_SR_RXNE) != (u16)RESET){

		if(userUsartInterrupt4 != NULL){
				userUsartInterrupt4((byte)USART4->regs->DR);
				return;
			}
		usart_irq(UART4);
	}

}

void __irq_uart5(void) {
	if ((USART5->regs->SR & USART_SR_RXNE) != (u16)RESET){

		if(userUsartInterrupt5 != NULL){
				userUsartInterrupt5((byte)USART5->regs->DR);
				return;
			}
		usart_irq(UART5);
	}

}
#endif


#ifdef CM9_DEBUG
//For debug
void TxDByteC(uint8 buf){

	   while (!usart_tx(USART2, &buf, 1))
	        ;
}

void TxDStringC(char *str)
{
	int i;
	for(i=0; str[i] ; i++)
	{
		TxDByteC(str[i]);
	}
}

void TxD_Dec_U8C(u8 bByte)
{
    u8 bTmp;
    bTmp = bByte/100;
    /*if(bTmp)*/ TxDByteC( bTmp+'0');
    bByte -= bTmp*100;
    bTmp = bByte/10;
    /*if(bTmp)*/ TxDByteC( bTmp+'0');
    bByte -= bTmp*10;
    TxDByteC( bByte+'0');
}

void TxDHex8C(u16 bSentData)
{
	u16 bTmp;

	bTmp = ((bSentData>>4)&0x000f) + (u8)'0';
	if(bTmp > '9') bTmp += 7;
	TxDByteC(bTmp);

	bTmp = (bSentData & 0x000f) + (u8)'0';
	if(bTmp > '9') bTmp += 7;
	TxDByteC(bTmp);
}
void TxDHex16C(u16 wSentData)
{
	TxDHex8C((wSentData>>8)&0x00ff );
	TxDHex8C( wSentData&0x00ff);
}

void TxDHex32C(u32 lSentData)
{
	TxDHex16C((lSentData>>16)&0x0000ffff );
	TxDHex16C( lSentData&0x0000ffff);
}
#endif


/* Failed ASSERT()s send out a message using this USART config. */
#ifndef ERROR_USART
#define ERROR_USART            USART2
#define ERROR_USART_CLK_SPEED  STM32_PCLK1
#define ERROR_USART_BAUD       57600//9600 //[ROBOTIS][CHANGE] 2013-04-22
#define ERROR_TX_PORT          GPIOA
#define ERROR_TX_PIN           2
#endif

/* If you define ERROR_LED_PORT and ERROR_LED_PIN, then a failed
 * ASSERT() will also throb() an LED connected to that port and pin.ERROR_LED_PIN
 */
#if defined(ERROR_LED_PORT) && defined(ERROR_LED_PIN)
#define HAVE_ERROR_LED
#endif

/**
 * @brief Disables all peripheral interrupts except USB and fades the
 *        error LED.
 */
/* (Called from exc.S with global interrupts disabled.) */
void __error(void) {
	//TxDStringC("Exception occur \r\n");

	//gpio_write_bit(GPIOB, 2, 0); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.

    /* Turn off peripheral interrupts */
    nvic_irq_disable_all();

    /* Turn off timers */
    timer_disable_all();

    /* Turn off ADC */
    adc_disable_all();

    /* Turn off all USARTs */
    usart_disable_all();

    /* Turn the USB interrupt back on so the bootloader keeps on functioning */
    nvic_irq_enable(NVIC_USB_HP_CAN_TX);
    nvic_irq_enable(NVIC_USB_LP_CAN_RX0);

    /* Reenable global interrupts */
    nvic_globalirq_enable();

    gpio_write_bit(GPIOB, 2, 0); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.
    gpio_write_bit(GPIOB, 9, 0); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.
    delay_us(100000);
    gpio_write_bit(GPIOB, 2, 1); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.
    gpio_write_bit(GPIOB, 9, 1); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.
    delay_us(100000);
    gpio_write_bit(GPIOB, 2, 0); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.
    gpio_write_bit(GPIOB, 9, 0); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.
    delay_us(100000);
    gpio_write_bit(GPIOB, 2, 1); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.
    gpio_write_bit(GPIOB, 9, 1); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.
    delay_us(100000);
    main(); //[ROBOTIS][ADD] 2013-04-23 re-executes main() again when exceptions occur
    //throb();

}

/** __cs3_start_c
 * @brief Print an error message on a UART upon a failed assertion
 *        and throb the error LED, if there is one defined.
 * @param file Source file of failed assertion
 * @param line Source line of failed assertion
 * @param exp String representation of failed assertion
 * @sideeffect Turns of all peripheral interrupts except USB.
 */
void _fail(const char* file, int line, const char* exp) {
#if 0
    /* Initialize the error USART */
    gpio_set_mode(ERROR_TX_PORT, ERROR_TX_PIN, GPIO_AF_OUTPUT_PP);
    usart_init(ERROR_USART);
    usart_set_baud_rate(ERROR_USART, ERROR_USART_CLK_SPEED, ERROR_USART_BAUD);

    /* Print failed assert message */
    usart_putstr(ERROR_USART, "ERROR: FAILED ASSERT(");
    usart_putstr(ERROR_USART, exp);
    usart_putstr(ERROR_USART, "): ");
    usart_putstr(ERROR_USART, file);
    usart_putstr(ERROR_USART, ": ");
    usart_putudec(ERROR_USART, line);
    usart_putc(ERROR_USART, '\n');
    usart_putc(ERROR_USART, '\r');
#endif
    /* Error fade */

    __error();
}

/**
 * @brief Fades the error LED on and off
 * @sideeffect Sets output push-pull on ERROR_LED_PIN.
 */
void throb(void) {
#ifdef HAVE_ERROR_LED
    int32  slope   = 1;
    uint32 CC      = 0x0000;
    uint32 TOP_CNT = 0x0200;
    uint32 i       = 0;

    gpio_set_mode(ERROR_LED_PORT, ERROR_LED_PIN, GPIO_OUTPUT_PP);
    /* Error fade. */
    while (1) {
        if (CC == TOP_CNT)  {
            slope = -1;
        } else if (CC == 0) {
            slope = 1;
        }

        if (i == TOP_CNT)  {
            CC += slope;
            i = 0;
        }

        if (i < CC) {
            gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 1);
        } else {
            gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 0);
        }
        i++;
    }
#else
    //gpio_write_bit(GPIOB, 2, 0); // [ROBOTIS][CHANGE] 2013-04-22 just LED on when errors occur.
    /* No error LED is defined; do nothing. */

    while (1)
        ;
#endif
}

