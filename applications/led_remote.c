#include "led_remote.h"
#include "stm32f4xx_mort2.h"
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include "hardware_stm_gpio.h"


// Constants ////////////////////
#define LED_SEG_DIG_0           0
#define LED_SEG_DIG_1           1
#define LED_SEG_DIG_2           2
#define LED_SEG_DIG_3           3
#define LED_SEG_A_PORT          PORT_D // D6
#define LED_SEG_A_PIN           6
#define LED_SEG_B_PORT          PORT_D // D2
#define LED_SEG_B_PIN           2
#define LED_SEG_C_PORT          PORT_B // B1
#define LED_SEG_C_PIN           1
#define LED_SEG_D_PORT          PORT_F // F4
#define LED_SEG_D_PIN           4
#define LED_SEG_E_PORT          PORT_B // B6
#define LED_SEG_E_PIN           6
#define LED_SEG_F_PORT          PORT_D // D7
#define LED_SEG_F_PIN           7
#define LED_SEG_G_PORT          PORT_B // B5   
#define LED_SEG_G_PIN           5
#define LED_SEG_DIG_0_PORT      PORT_D // D5
#define LED_SEG_DIG_0_PIN       5
#define LED_SEG_DIG_1_PORT      PORT_G  // G3
#define LED_SEG_DIG_1_PIN       3       
#define LED_SEG_DIG_2_PORT      PORT_G // G2       
#define LED_SEG_DIG_2_PIN       2       
#define LED_SEG_DIG_3_PORT      PORT_C // C7   
#define LED_SEG_DIG_3_PIN       7


// Blue: D4
#define BLUE_PORT       3 // D
#define BLUE_PIN        4
// Yellow: D3
#define YELLOW_PORT     3 // D
#define YELLOW_PIN      3
// Green: C10
#define GREEN_PORT      2 // C
#define GREEN_PIN       10
// White: C8
#define WHITE_PORT      2 // C
#define WHITE_PIN       8
// RGB Green: E2
#define RGB_GREEN_PORT  4 // E
#define RGB_GREEN_PIN   2
// RGB Red: E4
#define RGB_RED_PORT    4 // E
#define RGB_RED_PIN     4

// Variables
int led_display_values[4];

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
 * Sets the internal values for the segument display
 * However does not start the cycling of the leds!
 * @param values: 4 int values to be displayed, one for each digit
 */
int set_led_disp_vals(int values[4]) {
    int len = 4;
    for (int i=0; i<len; i++) {
        led_display_values[i] = values[i];
    }
}
/*
 * Seta single internal value for the segument display
 * However does not start the cycling of the leds!
 * @param digit: which digit to set
 * @param value: the single value to set
 */
int set_led_disp_val(int digit, int value) {
    if (digit < 0 || digit > 3) {
        fprintf(stderr, "%d is out of bounds for digit to set_led_disp_val()\n", digit);
        return -1;
    }
    // Set the value
    led_display_values[digit] = value;
    return 0;
}

/*
 * For the 4-digit led segment display, only 1 digit can be displayed at a time
 * cuz all digits are connected together internally
 * So need to cycle through each digit in order to display all 4 digits
 */
void cycle_led_display( void ) {
    // Calculate the current digit to be displayed on the led
    static int prev_digit = 0;
    int curr_digit = (prev_digit + 1) % 4; // Cycle 0 -> 1 -> 2 -> 3 -> 0 -> 1...
    prev_digit = curr_digit;

    // Set the led display
    set_seg_led(curr_digit, led_display_values[curr_digit]);
}

/*
 * Initialize pins for 7-segment display to outputs
 */
int init_seg_display( void ) 
{   
    // Initalize the 4 Digit Pins
    int digit_pin_init_val = 1; // Digit pins are off at HIGH
    initGPIOasMode(LED_SEG_DIG_0_PORT, LED_SEG_DIG_0_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, digit_pin_init_val, 0);   
    initGPIOasMode(LED_SEG_DIG_1_PORT, LED_SEG_DIG_1_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, digit_pin_init_val, 0);    
    initGPIOasMode(LED_SEG_DIG_2_PORT, LED_SEG_DIG_2_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, digit_pin_init_val, 0);    
    initGPIOasMode(LED_SEG_DIG_3_PORT, LED_SEG_DIG_3_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, digit_pin_init_val, 0);    

    // Initialize the A-G pins
    int seg_pin_init_val = 0; // A-G pins are off at LOW
    initGPIOasMode(LED_SEG_A_PORT, LED_SEG_A_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, seg_pin_init_val, 0);
    initGPIOasMode(LED_SEG_B_PORT, LED_SEG_B_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, seg_pin_init_val, 0);
    initGPIOasMode(LED_SEG_C_PORT, LED_SEG_C_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, seg_pin_init_val, 0);
    initGPIOasMode(LED_SEG_D_PORT, LED_SEG_D_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, seg_pin_init_val, 0);
    initGPIOasMode(LED_SEG_E_PORT, LED_SEG_E_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, seg_pin_init_val, 0);
    initGPIOasMode(LED_SEG_F_PORT, LED_SEG_F_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, seg_pin_init_val, 0);
    initGPIOasMode(LED_SEG_G_PORT, LED_SEG_G_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, seg_pin_init_val, 0);
}

/* 
 * Set the digit value
 * @param val: is int between 0-9
 */
int set_digit_value(int val) {
    // Initialize led segments based on the desired value
    led_segs_t led_segs = {.dp=false, .a=false, .b=false, .c=false, .d=false, .e=false, .f=false, .g=false};
    switch (val) {
        case -1:
            // Is off
            break;
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
            printf("[ERROR] led_remote.c: %d must be between -1 - 9\n", val);
            break;
    }

    // Set the values on the pins
    SETorCLEARGPIOoutput(LED_SEG_A_PORT, LED_SEG_A_PIN, led_segs.a);
    SETorCLEARGPIOoutput(LED_SEG_B_PORT, LED_SEG_B_PIN, led_segs.b);
    SETorCLEARGPIOoutput(LED_SEG_C_PORT, LED_SEG_C_PIN, led_segs.c);
    SETorCLEARGPIOoutput(LED_SEG_D_PORT, LED_SEG_D_PIN, led_segs.d);
    SETorCLEARGPIOoutput(LED_SEG_E_PORT, LED_SEG_E_PIN, led_segs.e);
    SETorCLEARGPIOoutput(LED_SEG_F_PORT, LED_SEG_F_PIN, led_segs.f);
    SETorCLEARGPIOoutput(LED_SEG_G_PORT, LED_SEG_G_PIN, led_segs.g);

    return 0; // success
}

/* 
 * Select one of the 4 digits in the 4-digit display to output to
 * @param selected_digit: is int between 0-3
 */
int select_digit(uint8_t selected_digit) {
    if (selected_digit < 0 || selected_digit > 3) {
        printf("[ERROR] led_remote.c: %d must be between 0-3\n", selected_digit);
        return -1; // error
    }

    // Set the user selected digit to false and all else as true
    bool digits[4] = {true, true, true, true};
    digits[selected_digit] = false;

    // Set the pin outputs
    SETorCLEARGPIOoutput(LED_SEG_DIG_0_PORT, LED_SEG_DIG_0_PIN, digits[0]);
    SETorCLEARGPIOoutput(LED_SEG_DIG_1_PORT, LED_SEG_DIG_1_PIN, digits[1]);
    SETorCLEARGPIOoutput(LED_SEG_DIG_2_PORT, LED_SEG_DIG_2_PIN, digits[2]);
    SETorCLEARGPIOoutput(LED_SEG_DIG_3_PORT, LED_SEG_DIG_3_PIN, digits[3]);


    return 0; // success
}

/* 
 * Sets a single digit to a value on the 7 segment LED display
 * @param digit: is int between 0-3 to select which of the 4 digits to output to
 * @param val: is int between 0-9 to set the digit value to display, -1 is off
 */
int set_seg_led(uint8_t digit, int val) {
    select_digit(digit);
    set_digit_value(val);

    return 0; // success
}

// Setting and clearing status LEDs
int set_blue_led(void) {
    SETorCLEARGPIOoutput(BLUE_PORT, BLUE_PIN, 1);
    return 0; // success
}
int clear_blue_led(void) {
    SETorCLEARGPIOoutput(BLUE_PORT, BLUE_PIN, 0);
    return 0; // success
}
int set_yellow_led(void) {
    SETorCLEARGPIOoutput(YELLOW_PORT, YELLOW_PIN, 1);
    return 0; // success
}
int clear_yellow_led(void) {
    SETorCLEARGPIOoutput(YELLOW_PORT, YELLOW_PIN, 0);
    return 0; // success
}
int set_green_led(void) {
    SETorCLEARGPIOoutput(GREEN_PORT, GREEN_PIN, 1);
    return 0; // success
}
int clear_green_led(void) {
    SETorCLEARGPIOoutput(GREEN_PORT, GREEN_PIN, 0);
    return 0; // success
}
int set_white_led(void) {
    SETorCLEARGPIOoutput(WHITE_PORT, WHITE_PIN, 1);
    return 0; // success
}
int clear_white_led(void) {
    SETorCLEARGPIOoutput(WHITE_PORT, WHITE_PIN, 0);
    return 0; // success
}
int set_rgb_green_led(void) {
    SETorCLEARGPIOoutput(RGB_GREEN_PORT, RGB_GREEN_PIN, 1);
    return 0; // success
}
int clear_rgb_green_led(void) {
    SETorCLEARGPIOoutput(RGB_GREEN_PORT, RGB_GREEN_PIN, 0);
    return 0; // success
}
int set_rgb_red_led(void) {
    SETorCLEARGPIOoutput(RGB_RED_PORT, RGB_RED_PIN, 1);
    return 0; // success
}
int clear_rgb_red_led(void) {
    SETorCLEARGPIOoutput(RGB_RED_PORT, RGB_RED_PIN, 0);
    return 0; // success
}


/* 
 * Initialize status leds
 */
int init_status_leds(void) {
    // mapping port number: 0->A, 1->B, ... 7->H
    // mode: 0-input, 1-output, 2-AF
    // open_drain: 0-pupd, 1-open drain
    // pupd: 0-floating, 1-pull up, 2-pull down
    // init_output: [0,1] initial value if in output mode 
    // alt_func: number of alternate function mode

    // Initalize all LEDs as output
    int initial_value = 0;
    initGPIOasMode(BLUE_PORT, BLUE_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, initial_value, 0);
    initGPIOasMode(YELLOW_PORT, YELLOW_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, initial_value, 0);
    initGPIOasMode(GREEN_PORT, GREEN_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, initial_value, 0);
    initGPIOasMode(WHITE_PORT, WHITE_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, initial_value, 0);
    initGPIOasMode(RGB_GREEN_PORT, RGB_GREEN_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, initial_value, 0);
    initGPIOasMode(RGB_RED_PORT, RGB_RED_PIN, MODE_OUT, OD_PUPD, PUPD_FLOAT, initial_value, 0);
    
    // Set initial states for the LEDs
    // set_blue_led();
    clear_blue_led();
    // set_yellow_led();
    clear_yellow_led();
    // set_green_led();
    clear_green_led();
    // set_white_led();
    clear_white_led();
    // set_rgb_green_led();
    clear_rgb_green_led();
    // set_rgb_red_led();
    clear_rgb_red_led();


    return 0; // success
}
