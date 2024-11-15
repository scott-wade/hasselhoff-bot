#include "led_remote.h"
#include "stm32f4xx_mort2.h"
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include "hardware_stm_gpio.h"


// Constants ////////////////////
#define LED_SEG_PORT    0
#define LED_SEG_DP      0
#define LED_SEG_A       0
#define LED_SEG_B       0
#define LED_SEG_C       0
#define LED_SEG_D       0
#define LED_SEG_E       0
#define LED_SEG_F       0
#define LED_SEG_G       0
#define LED_SEG_DIG_0   0
#define LED_SEG_DIG_1   0       
#define LED_SEG_DIG_2   0       
#define LED_SEG_DIG_3   0       


// Structs //////////////////////
typedef struct {
    bool dp; // decimal point
    bool a;
    bool b;
    bool c;
    bool d;
    bool e;
    bool f;
    bool g;
} led_segs_t;


// Function definitions //

/* 
 * Set the digit value
 * @param val: is int between 0-9
 */
int setDigitVal(uint8_t val) {
    // Initialize led segments based on the desired value
    led_segs_t led_segs = {.dp=false, .a=false, .b=false, .c=false, .d=false, .e=false, .f=false, .g=false};
    switch (val) {
        case 0:
            led_segs.a = true;
            led_segs.b = true;
            led_segs.c = true;
            led_segs.d = true;
            led_segs.e = true;
            led_segs.f = true;
            break;
        case 1:
            led_segs.b = true;
            led_segs.c = true;
            break;
        case 2:
            led_segs.a = true;
            led_segs.b = true;
            led_segs.d = true;
            led_segs.e = true;
            led_segs.g = true;
            break;
        case 3:
            led_segs.a = true;
            led_segs.b = true;
            led_segs.c = true;
            led_segs.d = true;
            led_segs.g = true;
            break;
        case 4:
            led_segs.b = true;
            led_segs.c = true;
            led_segs.f = true;
            led_segs.g = true;
            break;
        case 5:
            led_segs.a = true;
            led_segs.c = true;
            led_segs.d = true;
            led_segs.f = true;
            led_segs.g = true;
            break;
        case 6:
            led_segs.a = true;
            led_segs.c = true;
            led_segs.d = true;
            led_segs.e = true;
            led_segs.f = true;
            led_segs.g = true;
            break;
        case 7:
            led_segs.a = true;
            led_segs.b = true;
            led_segs.c = true;
            break;
        case 8:
            led_segs.a = true;
            led_segs.b = true;
            led_segs.c = true;
            led_segs.d = true;
            led_segs.e = true;
            led_segs.f = true;
            led_segs.g = true;
            break;
        case 9:
            led_segs.a = true;
            led_segs.b = true;
            led_segs.c = true;
            led_segs.d = true;
            led_segs.f = true;
            led_segs.g = true;
            break;
        default:
            printf("[ERROR] led_remote.c: %d must be between 0-9\n", val);
            break;
    }

    // Set the values on the pins
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_DP, led_segs.dp);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_A, led_segs.a);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_B, led_segs.b);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_C, led_segs.c);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_D, led_segs.d);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_E, led_segs.e);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_F, led_segs.f);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_G, led_segs.g);

    return 0; // success
}

/* 
 * Select one of the 4 digits in the 4-digit display to output to
 * @param selected_digit: is int between 0-3
 */
int selectDigit(uint8_t selected_digit) {
    if (selected_digit < 0 || selected_digit > 3) {
        printf("[ERROR] led_remote.c: %d must be between 0-3\n", selected_digit);
        return -1; // error
    }

    // Set the user selected digit to false and all else as true
    bool digits[4] = {true, true, true, true};
    digits[selected_digit] = false;

    // Set the pin outputs
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_DIG_0, digits[0]);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_DIG_1, digits[1]);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_DIG_2, digits[2]);
    SETorCLEARGPIOoutput(LED_SEG_PORT, LED_SEG_DIG_3, digits[3]);

    return 0; // success
}

/* 
 * Sets a single digit to a value on the 7 segment LED display
 * @param digit: is int between 0-3 to select which of the 4 digits to output to
 * @param val: is int between 0-9 to set the digit value to display
 */
int setSegLED(uint8_t digit, uint8_t val) {
    selectDigit(digit);
    setDigitVal(val);

    return 0; // success
}