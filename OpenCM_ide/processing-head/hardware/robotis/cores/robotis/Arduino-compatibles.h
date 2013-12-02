/*
 * Arduino-compatibles.h
 *
 *  Created on: 2013. 4. 25.
 *      Author: in2storm
 */

#ifndef ARDUINO_COMPATIBLES_H_
#define ARDUINO_COMPATIBLES_H_

#include <stdlib.h>
#include <math.h>


//[START] wirish_time.h
#include "boards.h" //it gets to be ported board-specific header file[CM9XX.h]
#include "systick.h"
#include "delay.h"
//[END] wirish_time.h

//[START] io.h
#include "gpio.h"
#include "adc.h"
#include "wirish.h"
//[END] io.h


/*********************************************************************************************************
 * ported from wirish_math.h(LEAFLABS) www.leaflabs.com
 * ROBOTIS,.LTD.
 ********************************************************************************************************* */

/**
 * @brief Initialize the pseudo-random number generator.
 * @param seed the number used to initialize the seed; cannot be zero.
 */
void randomSeed(unsigned int seed);

/**
 * @brief Generate a pseudo-random number with upper bound.
 * @param max An upper bound on the returned value, exclusive.
 * @return A pseudo-random number in the range [0,max).
 * @see randomSeed()
 */
long random(long max);

/**
 * @brief Generate a pseudo-random number with lower and upper bounds.
 * @param min Lower bound on the returned value, inclusive.
 * @param max Upper bound on the returned value, exclusive.
 * @return A pseudo-random number in the range [min, max).
 * @see randomSeed()
 */
long random(long min, long max);

/**
 * @brief Remap a number from one range to another.
 *
 * That is, a value equal to fromStart gets mapped to toStart, a value
 * of fromEnd to toEnd, and other values are mapped proportionately.
 *
 * Does not constrain value to lie within [fromStart, fromEnd].
 *
 * If a "start" value is larger than its corresponding "end", the
 * ranges are reversed, so map(n, 1, 10, 10, 1) would reverse the
 * range [1,10].
 *
 * Negative numbers may appear as any argument.
 *
 * @param value the value to map.
 * @param fromStart the beginning of the value's current range.
 * @param fromEnd the end of the value's current range.
 * @param toStart the beginning of the value's mapped range.
 * @param toEnd the end of the value's mapped range.
 * @return the mapped value.
 */
static inline long map(long value, long fromStart, long fromEnd,
                long toStart, long toEnd) {
    return (value - fromStart) * (toEnd - toStart) / (fromEnd - fromStart) +
        toStart;
}

#define PI          3.1415926535897932384626433832795
#define HALF_PI     1.5707963267948966192313216916398
#define TWO_PI      6.283185307179586476925286766559
#define DEG_TO_RAD  0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)                ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg)            ((deg)*DEG_TO_RAD)
#define degrees(rad)            ((rad)*RAD_TO_DEG)
#define sq(x)                   ((x)*(x))

/* undefine stdlib's abs if encountered */
#ifdef abs
#undef abs
#endif
#define abs(x) (((x) > 0) ? (x) : -(x))

/* Following are duplicate declarations (with Doxygen comments) for
 * some of the math.h functions; this is for the convenience of the
 * Sphinx docs.
 */

/**
 * Compute the cosine of an angle, in radians.
 * @param x The radian measure of the angle.
 * @return The cosine of x.  This value will be between -1 and 1.
 */
double cos(double x);

/**
 * Compute the sine of an angle, in radians.
 * @param x The radian measure of the angle.
 * @return The sine of x.  This value will be between -1 and 1.
 */
double sin(double x);

/**
 * Compute the tangent of an angle, in radians.
 * @param x The radian measure of the angle.
 * @return The tangent of x.  There are no limits on the return value
 * of this function.
 */
double tan(double x);

/**
 * Compute the square root of a number.
 * @param x The number whose square root to find.  This value cannot
 * be negative.
 * @return The square root of x.  The return value is never negative.
 */
double sqrt(double x);

/**
 * Compute an exponentiation.
 * @param x the base. This value cannot be zero if y <= 0.  This value
 * cannot be negative if y is not an integral value.
 * @param y the exponent.
 * @return x raised to the power y.
 */
double pow(double x, double y);



/*********************************************************************************************************
 * ported from wirish_time.h(LEAFLABS) www.leaflabs.com
 * ROBOTIS,.LTD.
 ********************************************************************************************************* */

#define US_PER_MS               1000

/**
 * Returns time (in milliseconds) since the beginning of program
 * execution. On overflow, restarts at 0.
 * @see micros()
 */
static inline uint32 millis(void) {
    return systick_uptime();
}

/**
 * Returns time (in microseconds) since the beginning of program
 * execution.  On overflow, restarts at 0.
 * @see millis()
 */
static inline uint32 micros(void) {
    uint32 ms;
    uint32 cycle_cnt;
    uint32 res;

    do {
        cycle_cnt = systick_get_count();
        ms = millis();
    } while (ms != millis());

    /* SYSTICK_RELOAD_VAL is 1 less than the number of cycles it
       actually takes to complete a SysTick reload */
    res = (ms * US_PER_MS) +
        (SYSTICK_RELOAD_VAL + 1 - cycle_cnt) / CYCLES_PER_MICROSECOND;

    return res;
}

/**
 * Delay for at least the given number of milliseconds.
 *
 * Interrupts, etc. may cause the actual number of milliseconds to
 * exceed ms.  However, this function will return no less than ms
 * milliseconds from the time it is called.
 *
 * @param ms the number of milliseconds to delay.
 * @see delayMicroseconds()
 */
void delay(unsigned long ms);

/**
 * Delay for at least the given number of microseconds.
 *
 * Interrupts, etc. may cause the actual number of microseconds to
 * exceed us.  However, this function will return no less than us
 * microseconds from the time it is called.
 *
 * @param us the number of microseconds to delay.
 * @see delay()
 */
void delayMicroseconds(uint32 us);





/*********************************************************************************************************
 * ported from io.h(LEAFLABS) www.leaflabs.com
 * analog/digital/other port handling methods are also ported from wirish_analog.h wirish_digital.h wirish_shift.h
 * 2013-04-25 ROBOTIS,.LTD.
 ********************************************************************************************************* */



/**
 * Specifies a GPIO pin behavior.
 * @see pinMode()
 */
typedef enum WiringPinMode {
    OUTPUT, /**< Basic digital output: when the pin is HIGH, the
               voltage is held at +3.3v (Vcc) and when it is LOW, it
               is pulled down to ground. */

    OUTPUT_OPEN_DRAIN, /**< In open drain mode, the pin indicates
                          "low" by accepting current flow to ground
                          and "high" by providing increased
                          impedance. An example use would be to
                          connect a pin to a bus line (which is pulled
                          up to a positive voltage by a separate
                          supply through a large resistor). When the
                          pin is high, not much current flows through
                          to ground and the line stays at positive
                          voltage; when the pin is low, the bus
                          "drains" to ground with a small amount of
                          current constantly flowing through the large
                          resistor from the external supply. In this
                          mode, no current is ever actually sourced
                          from the pin. */

    INPUT, /**< Basic digital input. The pin voltage is sampled; when
              it is closer to 3.3v (Vcc) the pin status is high, and
              when it is closer to 0v (ground) it is low. If no
              external circuit is pulling the pin voltage to high or
              low, it will tend to randomly oscillate and be very
              sensitive to noise (e.g., a breath of air across the pin
              might cause the state to flip). */

    INPUT_ANALOG, /**< This is a special mode for when the pin will be
                     used for analog (not digital) reads.  Enables ADC
                     conversion to be performed on the voltage at the
                     pin. */

    INPUT_PULLUP, /**< The state of the pin in this mode is reported
                     the same way as with INPUT, but the pin voltage
                     is gently "pulled up" towards +3.3v. This means
                     the state will be high unless an external device
                     is specifically pulling the pin down to ground,
                     in which case the "gentle" pull up will not
                     affect the state of the input. */

    INPUT_PULLDOWN, /**< The state of the pin in this mode is reported
                       the same way as with INPUT, but the pin voltage
                       is gently "pulled down" towards 0v. This means
                       the state will be low unless an external device
                       is specifically pulling the pin up to 3.3v, in
                       which case the "gentle" pull down will not
                       affect the state of the input. */

    INPUT_FLOATING, /**< Synonym for INPUT. */

    PWM, /**< This is a special mode for when the pin will be used for
            PWM output (a special case of digital output). */

    PWM_OPEN_DRAIN, /**< Like PWM, except that instead of alternating
                       cycles of LOW and HIGH, the voltage on the pin
                       consists of alternating cycles of LOW and
                       floating (disconnected). */
} WiringPinMode;

/**
 * Configure behavior of a GPIO pin.
 *
 * @param pin Number of pin to configure.
 * @param mode Mode corresponding to desired pin behavior.
 * @see WiringPinMode
 */
void pinMode(uint8 pin, WiringPinMode mode);

/**
 * Writes a (digital) value to a pin.  The pin must have its
 * mode set to OUTPUT or OUTPUT_OPEN_DRAIN.
 *
 * @param pin Pin to write to.
 * @param value Either LOW (write a 0) or HIGH (write a 1).
 * @see pinMode()
 */
void digitalWrite(uint8 pin, uint8 value);

/**
 * Read a digital value from a pin.  The pin must have its mode set to
 * one of INPUT, INPUT_PULLUP, and INPUT_PULLDOWN.
 *
 * @param pin Pin to read from.
 * @return LOW or HIGH.
 * @see pinMode()
 */
uint32 digitalRead(uint8 pin);

/**
 * Read an analog value from pin.  This function blocks during ADC
 * conversion, and has 12 bits of resolution.  The pin must have its
 * mode set to INPUT_ANALOG.
 *
 * @param pin Pin to read from.
 * @return Converted voltage, in the range 0--4095, (i.e. a 12-bit ADC
 *         conversion).
 * @see pinMode()
 */
uint16 analogRead(uint8 pin);

/**
 * Toggles the digital value at the given pin.
 *
 * The pin must have its mode set to OUTPUT.
 *
 * @param pin the pin to toggle.  If the pin is HIGH, set it LOW.  If
 * it is LOW, set it HIGH.
 *
 * @see pinMode()
 */
void togglePin(uint8 pin);

/**
 * Toggle the LED.
 *
 * If the LED is on, turn it off.  If it is off, turn it on.
 *
 * The LED must its mode set to OUTPUT. This can be accomplished
 * portably over all LeafLabs boards by calling pinMode(BOARD_LED_PIN,
 * OUTPUT) before calling this function.
 *
 * @see pinMode()
 */
static inline void toggleLED() {
    togglePin(BOARD_LED_PIN);
}
#if defined(BOARD_CM904)
/**
 * If the button is currently pressed, waits until the button is no
 * longer being pressed, and returns true.  Otherwise, returns false.
 *
 * The button pin must have its mode set to INPUT.  This can be
 * accomplished portably over all LeafLabs boards by calling
 * pinMode(BOARD_BUTTON_PIN, INPUT).
 *
 * @see pinMode()
 */
uint8 isButtonPressed();

/**
 * Wait until the button is pressed and released, timing out if no
 * press occurs.
 *
 * The button pin must have its mode set to INPUT.  This can be
 * accomplished portably over all LeafLabs boards by calling
 * pinMode(BOARD_BUTTON_PIN, INPUT).
 *
 * @param timeout_millis Number of milliseconds to wait until the
 * button is pressed.  If timeout_millis is left out (or 0), wait
 * forever.
 *
 * @return true, if the button was pressed; false, if the timeout was
 * reached.
 *
 * @see pinMode()
 */
uint8 waitForButtonPress(uint32 timeout_millis=0);
#endif


/**
 * Shift out a byte of data, one bit at a time.
 *
 * This function starts at either the most significant or least
 * significant bit in a byte value, and shifts out each byte in order
 * onto a data pin.  After each bit is written to the data pin, a
 * separate clock pin is pulsed to indicate that the new bit is
 * available.
 *
 * @param dataPin  Pin to shift data out on
 * @param clockPin Pin to pulse after each bit is shifted out
 * @param bitOrder Either MSBFIRST (big-endian) or LSBFIRST (little-endian).
 * @param value    Value to shift out
 */

void shiftOut(uint8 dataPin, uint8 clockPin, uint8 bitOrder, uint8 value);
/*
 * Convert integer value to sting value
 * atoi() is included compiler's libraries but itoa() is not included
 * itoa() added in Arduino-compatibles temporarily.
 * */
void itoa(int value, char* str, int base);

#endif /* ARDUINO_COMPATIBLES_H_ */
