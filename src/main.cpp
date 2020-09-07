#include <Arduino.h>
#include <Wire.h>
#include "bsp.h"

#define RTC_INT_PIN       (2)
#define BUTTON3_PIN       (3)

RtcDateTime time;
volatile bool alarm_goes = false;

void update_time(RtcDateTime *t);

// void ISR_ATTR Alarm2_ISR()
void Alarm2_ISR()
{
    alarm_goes = true;
}

void Button3_ISR()
{
    alarm_goes = true;
}


// void setup() {
int main() {

    init(); //From <Aruino.h>/<wiring.c>

    //Configure pins
    pinMode(RTC_INT_PIN, INPUT_PULLUP);
    pinMode(BUTTON3_PIN, INPUT_PULLUP);

    //Run I2C
    Wire.begin();

    //Configure Light sensor
    lsensor_init();

    //Enable power and configure LED display
    display_pwr(1);
    delay(300u);
    display_init();
    display_set_intensity(MIN_INTENSITY+2);

    //Init RTC
    alarm_goes = false;
    rtc_init();
    update_time(&time);
    rtc_alarm_every_minute();
    attachInterrupt(digitalPinToInterrupt(RTC_INT_PIN), Alarm2_ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON3_PIN), Button3_ISR, FALLING);
    delay(100);
//}
    //Main loop

// void loop() {
   for(;;) {
    if (alarm_goes) {
        rtc_alarm2_check();
        update_time(&time);
        alarm_goes = false;
    }

// for(;;) {
    delay(500);
    display_point(1, true);
    delay(500);
    display_point(1, false);
  // }
    }
}



void update_time(RtcDateTime *t)
{
  rtc_get_time(t);
  display_time(t);
}