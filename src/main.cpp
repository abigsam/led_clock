#include <Arduino.h>
#include <Wire.h>
#include "bsp.h"

#define RTC_INT_PIN       (2)
// #define BUTTON3_PIN       (3)

volatile bool alarm_goes = false;
uint16_t light = 0u;
uint8_t bmode_cnt = 0u;


void Alarm2_ISR()
{
    alarm_goes = true;
}

// void Button3_ISR()
// {
//     alarm_goes = true;
// }

typedef enum {
    DISPLAY_TIME = 0,
    DISPLAY_EDIT_TIME,
    DISPLAY_LUX,
    DISPLAY_MODE_NUM
} mode_t;

mode_t curr_mode = DISPLAY_TIME;

int main() {

    init(); //From <Aruino.h>/<wiring.c>

    //Configure pins
    pinMode(RTC_INT_PIN, INPUT_PULLUP);
    // pinMode(BUTTON3_PIN, INPUT_PULLUP);

    //Run I2C
    Wire.begin();

    //Init BSP
    bsp_init();

    //Enable power and configure LED display
    display_pwr(1);
    delay(300u);
    display_init();
    bsp_set_intens();

    //Init RTC
    alarm_goes = false;
    rtc_init();
    bsp_update_time();
    rtc_alarm_every_minute();
    attachInterrupt(digitalPinToInterrupt(RTC_INT_PIN), Alarm2_ISR, FALLING);
    // attachInterrupt(digitalPinToInterrupt(BUTTON3_PIN), Button3_ISR, FALLING);
    delay(100);

    //Main loop
    for(;;) {
        if (DISPLAY_TIME == curr_mode) {
            //Control display intesity
            bsp_set_intens();
            //Check if alarm goes
            if (alarm_goes) {
                rtc_alarm2_check();
                bsp_update_time();
                alarm_goes = false;
            }
        }
        if (DISPLAY_EDIT_TIME == curr_mode) {
            bsp_edit_time();
            curr_mode = DISPLAY_TIME;
        }
        if (DISPLAY_LUX == curr_mode) {
            light = bsp_lsensor_read();
            display_set(light);
        }

        //Check button Mode
        if (bsp_read_button(BUTTON_MODE))
            bmode_cnt++;
        else
            bmode_cnt = 0;
        
        //Switch display mode if need
        if (bmode_cnt > 4) {
            bmode_cnt = 0u;
            curr_mode = (DISPLAY_MODE_NUM == curr_mode) ? DISPLAY_TIME : (mode_t)(curr_mode+1);

            if (DISPLAY_EDIT_TIME == curr_mode) {
                display_set((char *)"E   ", 0);
                display_set_intensity(MAX_INTENSITY);
            }
            if (DISPLAY_LUX == curr_mode) {
                display_set((char *)"L   ", 0);
                display_set_intensity(MAX_INTENSITY);
            }
            if (DISPLAY_TIME == curr_mode) {
                display_set((char *)"T   ", 0);
                alarm_goes = true;
            }
        }

        //Blink decimal point
        delay(500);
        display_point(1, true);
        delay(500);
        display_point(1, false);
    }
}



