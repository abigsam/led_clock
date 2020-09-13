



#ifndef __BSP_H
#define __BSP_H

#include <Arduino.h>
#include "display.h"
#include "rtc.h"
// #include "light_sensor.h"

#define LSENSOR_PIN             (A3)  //OR PC3/ADC3
#define BUTTON_MODE_PIN         (3)   //OR PD3
#define BUTTON_PLUS_PIN         (5)   //OR PD5
#define BUTTON_MINUS_PIN        (6)   //OR PD6

#define LOW_LUX_SET         (4u)
#define HIGH_LUX_SET        (100u)

typedef enum {
    BUTTON_NONE = 0,
    BUTTON_MODE,
    BUTTON_PLUS,
    BUTTON_MINUS
} button_t;


void bsp_init();
uint8_t bsp_read_button(button_t btn);
uint16_t bsp_lsensor_read();
void bsp_set_intens();
void bsp_boost_conrol(bool state);
void bsp_charge_control(bool state);
bool bsp_read_charge();
bool bsp_read_usb_pwr();
void bsp_update_time();
void bsp_edit_time();

// void bsp_display_time();


#endif //__BSP_H