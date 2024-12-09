#include "led_remote.h"
#include "state_machine_remote.h"
#include "state_machine_sub.h"
#include "stm32f4xx_mort2.h"
#include <cstdint>
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include "hardware_stm_gpio.h"
#include "state_machine_SPI.h"
#include "timer_queue_remote.h"


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
#define BLUE_PORT       PORT_D // D
#define BLUE_PIN        4
// Yellow: D3
#define YELLOW_PORT     PORT_D // D
#define YELLOW_PIN      3
// Green: C10
#define GREEN_PORT      PORT_C // C
#define GREEN_PIN       10
// White: C8
#define WHITE_PORT      PORT_C // C
#define WHITE_PIN       8
// RGB Green: E2
#define RGB_GREEN_PORT  PORT_E // E
#define RGB_GREEN_PIN   2
// RGB Red: E4
#define RGB_RED_PORT    PORT_E // E
#define RGB_RED_PIN     4

#define CNT_DOWN_DIG_0  0 // Disp digit for countdown timer
#define CNT_DOWN_DIG_1  1 // Disp digit for countdown timer
#define COUTNDOWN       99 // Value to count down from 


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
    clear_all_leds();

    return 0; // success
}


/**
 * Clear all LEDs to off
 */
void clear_all_leds(void) {
    clear_blue_led();
    clear_yellow_led();
    clear_green_led();
    clear_white_led();
    clear_rgb_green_led();
    clear_rgb_red_led();
}

/*
 * Welcome state for the remote
 * Toggle the LEDs
 */
void welcome_remote (void)
{
    // Light up LED status lights
    static int led_i = 0;
    static bool reverse = false; // Go in reverse direction

    // LED labels
    // 1. Power: RGB
    // 2. Comms: yellow
    // 3. Target: Blue
    // 4. Landing: Green
    // 5. Driving: white
    switch(led_i) {
        case 0:
            // RGB on
            clear_all_leds();
            set_rgb_green_led();
            set_rgb_red_led();
            reverse = false; // Go up
            break;
        case 1:
            // Yellow on
            clear_all_leds();
            set_yellow_led();
            break;
        case 2:
            // Blue on
            clear_all_leds();
            set_blue_led();
            break;
        case 3:
            // Green on
            clear_all_leds();
            set_green_led();
            break;
        case 4:
            // White on
            clear_all_leds();
            set_white_led();
            reverse = true; // Go down
            break;
    }

    // Go forwards and backwards along the leds
    if (reverse)
        led_i--;
    else
        led_i++;
    
    //////////////////////////////////////////////////////////
    // Change led display numbers
    static int disp_i = 0;
    int disp_vals[4] = {disp_i, disp_i, disp_i, disp_i};
    set_led_disp_vals(disp_vals);
    disp_i = (disp_i+1)%10;
}

// Display timer that counts down in time
int countdown_timer (void) {
    static int count = COUTNDOWN; // Starting count
    int blink_count = 10; // How many times to blink before exiting state

    int first_dig, second_dig;
    /* 
     * 0-99 shows the number
     * Negative numbers will blink LEDs at 00 to indicate game over!
     */
    if (count >= 0) {
        // If value changed, set the led display value
        first_dig = count / 10; // Integer division
        second_dig = count % 10; // Remainder
    } else if (count <= -1*blink_count) {
        // Game over! Exit driving state
        count = COUTNDOWN; // Reset count to starting value
        // Driving -> Welcome
        sched_event(WELCOME_REMOTE);
        // Notify sub about timeout
        requestSpiTransmit_remote(RESET_MSG, 0, NULL); // send reset message
        return 0; // Exit
    } else {
        // Negative numbers is game over and leds will flash on and off
        if (count%2 == 0) {
            // Even negatives will turn it off
            first_dig = -1; // off
            second_dig = -1; // off
        } else {
            first_dig = 0; // zero
            second_dig = 0; // zero
        }
    }
    // Set the values on the led display
    set_led_disp_val(CNT_DOWN_DIG_0, first_dig);
    set_led_disp_val(CNT_DOWN_DIG_1, second_dig);

    // Decrement counter
    count--; 

    return count;
}


/**
 * Read the sub's status
 * Update status LEDs
 * Check for out of sync issues
 */
void read_sub_status(void) {
    // IR status[bit 0], sub state[bit 1-2], power (?)
    uint8_t is_target_detected, sub_state;

    // Set status LED is target is detected, only in drive/land states
    if ((remote_state == DRIVE_REMOTE) || 
        (remote_state == LAND_REMOTE)) {
            is_target_detected = sub_status & 0b01;
            if (is_target_detected)
                set_blue_led();
            else
                clear_blue_led();
        }

    // Get sub state
    sub_state = (sub_status >> 1) & 0b11;
    // Sub state is Idle (00), Welcome (01), Drive (10), Land (11)
    if (sub_state == 0b01 && remote_state != WELCOME_REMOTE) {
        // Sub in welcome state
        fprintf(stderr, "[ERROR] OUT OF SYNC: Sub is in welcome state!\n");
    } else if (sub_state == 0b10 && remote_state != DRIVE_REMOTE) {
        // Sub in drive state
        fprintf(stderr, "[ERROR] OUT OF SYNC: Sub is in drive state!\n");
    } else if (sub_state == 0b11 && remote_state != LAND_REMOTE) {
        // Sub in drive state
        fprintf(stderr, "[ERROR] OUT OF SYNC: Sub is in land state!\n");
    }
}