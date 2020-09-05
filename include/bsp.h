



#ifndef __BSP_H
#define __BSP_H

#include <Arduino.h>
// #include "display.h"
// #include "rtc.h"

typedef enum {
    BUTTON_NONE = 0,
    BUTTON_SET,
    BUTTON_PLUS,
    BUTTON_MINUS
} button_t;


void bsp_init();
uint8_t bsp_read_button(button_t btn);
uint8_t bsp_read_light();
void bsp_boost_conrol(bool state);
void bsp_charge_control(bool state);
bool bsp_read_charge();
bool bsp_read_usb_pwr();

// void bsp_display_time();


#endif //__BSP_H