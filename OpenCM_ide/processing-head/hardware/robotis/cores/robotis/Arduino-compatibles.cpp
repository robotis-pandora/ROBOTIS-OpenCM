/*
 * Arduino-compatibles.cpp
 *
 *  Created on: 2013. 4. 25.
 *      Author: [ROBOTIS,.LTD.] in2storm
 */



#include "Arduino-compatibles.h"
/*********************************************************************************************************
 * ported from wirish_math.h(LEAFLABS) www.leaflabs.com
 * ROBOTIS,.LTD.
 ********************************************************************************************************* */

void randomSeed(unsigned int seed) {
    if (seed != 0) {
        srand(seed);
    }
}

long random(long howbig) {
    if (howbig == 0) {
        return 0;
    }

    return rand() % howbig;
}

long random(long howsmall, long howbig) {
    if (howsmall >= howbig) {
        return howsmall;
    }

    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}


/*********************************************************************************************************
 * ported from wirish_time.h(LEAFLABS) www.leaflabs.com
 * ROBOTIS,.LTD.
 ********************************************************************************************************* */


void delay(unsigned long ms) {
    uint32 i;
    for (i = 0; i < ms; i++) {
        delayMicroseconds(1000);
    }
}

void delayMicroseconds(uint32 us) {
    delay_us(us);
}

/*********************************************************************************************************
 * ported from io.h(LEAFLABS) www.leaflabs.com
 * analog/digital/other port handling methods are also ported from wirish_analog.h wirish_digital.h wirish_shift.h
 * 2013-04-25 ROBOTIS,.LTD.
 ********************************************************************************************************* */


/* Assumes that the ADC has been initialized and that the pin is set
 * to INPUT_ANALOG */
uint16 analogRead(uint8 pin) {
    const adc_dev *dev = PIN_MAP[pin].adc_device;
    if (dev == NULL) {
        return 0;
    }

    return adc_read(dev, PIN_MAP[pin].adc_channel);
}



void pinMode(uint8 pin, WiringPinMode mode) {
    gpio_pin_mode outputMode;
    boolean pwm = false;

    if (pin >= BOARD_NR_GPIO_PINS) {
        return;
    }

    switch(mode) {
    case OUTPUT:
        outputMode = GPIO_OUTPUT_PP;
        break;
    case OUTPUT_OPEN_DRAIN:
        outputMode = GPIO_OUTPUT_OD;
        break;
    case INPUT:
    case INPUT_FLOATING:
        outputMode = GPIO_INPUT_FLOATING;
        break;
    case INPUT_ANALOG:
        outputMode = GPIO_INPUT_ANALOG;
        break;
    case INPUT_PULLUP:
        outputMode = GPIO_INPUT_PU;
        break;
    case INPUT_PULLDOWN:
        outputMode = GPIO_INPUT_PD;
        break;
    case PWM:
        outputMode = GPIO_AF_OUTPUT_PP;
        pwm = true;
        break;
    case PWM_OPEN_DRAIN:
        outputMode = GPIO_AF_OUTPUT_OD;
        pwm = true;
        break;
    default:
        ASSERT(0);
        return;
    }

    gpio_set_mode(PIN_MAP[pin].gpio_device, PIN_MAP[pin].gpio_bit, outputMode);

    if (PIN_MAP[pin].timer_device != NULL) {
        /* Enable/disable timer channels if we're switching into or
         * out of PWM. */
        timer_set_mode(PIN_MAP[pin].timer_device,
                       PIN_MAP[pin].timer_channel,
                       pwm ? TIMER_PWM : TIMER_DISABLED);
    }
}


uint32 digitalRead(uint8 pin) {
    if (pin >= BOARD_NR_GPIO_PINS) {
        return 0;
    }

    return gpio_read_bit(PIN_MAP[pin].gpio_device, PIN_MAP[pin].gpio_bit) ?
        HIGH : LOW;
}

void digitalWrite(uint8 pin, uint8 val) {
    if (pin >= BOARD_NR_GPIO_PINS) {
        return;
    }

    gpio_write_bit(PIN_MAP[pin].gpio_device, PIN_MAP[pin].gpio_bit, val);
}

void togglePin(uint8 pin) {
    if (pin >= BOARD_NR_GPIO_PINS) {
        return;
    }

    gpio_toggle_bit(PIN_MAP[pin].gpio_device, PIN_MAP[pin].gpio_bit);
}
#if defined(BOARD_CM904)
#define BUTTON_DEBOUNCE_DELAY 1

uint8 isButtonPressed() {
    if (digitalRead(BOARD_BUTTON_PIN)) {
        delay(BUTTON_DEBOUNCE_DELAY);
        while (digitalRead(BOARD_BUTTON_PIN))
            ;
        return true;
    }
    return false;
}

uint8 waitForButtonPress(uint32 timeout) {
    uint32 start = millis();
    uint32 time;
    if (timeout == 0) {
        while (!isButtonPressed())
            ;
        return true;
    }
    do {
        time = millis();
        /* properly handle wrap-around */
        if ((start > time && time + (0xffffffffU - start) > timeout) ||
            time - start > timeout) {
            return false;
        }
    } while (!isButtonPressed());
    return true;
}
#endif

void shiftOut(uint8 dataPin, uint8 clockPin, uint8 bitOrder, uint8 val) {
    int i;

    for (i = 0; i < 8; i++) {
        if (bitOrder == LSBFIRST) {
            digitalWrite(dataPin, !!(val & (1 << i)));
        } else {
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));
        }

        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}

// itoa function is referenced from the following site
// http://forums.leaflabs.com/topic.php?id=177
void strreverse(char* begin, char* end) {
  char aux;
  while(end>begin)
    aux=*end, *end--=*begin, *begin++=aux;
}
void itoa(int value, char* str, int base) {
  static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  char* wstr=str;
  int sign;

  // Validate base
  if (base<2 || base>35) {
    *wstr='\0';
    return;
  }

  // Take care of sign
  if ((sign=value) < 0)
    value = -value;

  // Conversion. Number is reversed.
  do
    *wstr++ = num[value%base];
  while(value/=base);

  if(sign<0)
    *wstr++='-';

  *wstr='\0';

  // Reverse string
  strreverse(str,wstr-1);
}




