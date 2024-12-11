#ifndef __REMOTE_STATE_CALLBACKS_H_
#define __REMOTE_STATE_CALLBACKS_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Constants ------------------------------------------------------------------*/
/* Period of periodically occuring tasks */
#define DISPLAY_CYCLE_PERIOD_MS         1
#define READ_DEPTH_PERIOD_MS            100
#define START_ADC_DELAY_MS              100
#define WELCOME_PERIOD_MS               500
#define RESET_PERIOD_MS                 200
#define READ_UX_PERIOD_MS               100
#define COUNTDOWN_TIMER_PERIOD_MS       1000 // 1 sec

#define JOY_ACTIVE_ZONE         500

/* Functions ------------------------------------------------------------------*/
// Main STATE events
void remote_init_callback(void);
void remote_welcome_callback(void);
void remote_drive_callback(void);
void remote_land_callback(void);
void remote_reset_callback(void);

// AUXILIARY EVENTS
void remote_start_adc_callback(void);
void remote_read_UX_callback(void);
void remote_led_display_callback(void);
void remote_countdown_timer(void);



#ifdef __cplusplus
}
#endif

#endif /*__STATE_MACHINE_REMOTE_H */