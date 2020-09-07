#include <Arduino.h>
#include <Wire.h>
#include "bsp.h"

#define RTC_INT_PIN       (2)
// #define BUTTON3_PIN       (3)

RtcDateTime time;
volatile bool alarm_goes = false;
uint16_t light = 0u;
uint8_t bmode_cnt = 0u;

void update_time(RtcDateTime *t);


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
    DISPLAY_LUX
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
    update_time(&time);
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
                update_time(&time);
                alarm_goes = false;
            }
        }
        if (DISPLAY_LUX == curr_mode) {
            light = bsp_lsensor_read();
            display_set(light);
        }

        //Check buttons
        if (bsp_read_button(BUTTON_MODE))
            bmode_cnt++;
        else
            bmode_cnt = 0;
        
        //Switch display mode if need
        if (bmode_cnt > 4) {
            bmode_cnt = 0u;
            if (DISPLAY_TIME == curr_mode) {
                // display_set((char *)"T");
                display_set_intensity(MAX_INTENSITY);
                curr_mode = DISPLAY_LUX;
            } else {
                // display_set((char *)"L");
                alarm_goes = true;
                curr_mode = DISPLAY_TIME;
            }
        }


        //Blink decimal point
        delay(500);
        display_point(1, true);
        delay(500);
        display_point(1, false);
    }
}



void update_time(RtcDateTime *t)
{
  rtc_get_time(t);
  display_time(t);
}