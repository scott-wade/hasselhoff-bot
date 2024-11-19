#ifndef __LED_REMOTE_H_
#define __LED_REMOTE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Function definitions */
int init_status_leds(void);
int setBlueLED(void);
int clearBlueLED(void);
int setYellowLED(void);
int clearYellowLED(void);
int setGreenLED(void);
int clearGreenLED(void);
int setWhiteLED(void);
int clearWhiteLED(void);
int setRgbGreenLED(void);
int clearRgbGreenLED(void);
int setRgbRedLED(void);
int clearRgbRedLED(void);

#ifdef __cplusplus
}
#endif

#endif /*__LED_REMOTE_H */