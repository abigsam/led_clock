#include <Arduino.h>
#include <Wire.h>
#include "display.h"
#include "rtc.h"
#include "bsp.h"

#define RTC_INT_PIN       (2)
#define BUTTON3_PIN       (3)

RtcDateTime time;
volatile bool alarm_goes = false;

void update_time(RtcDateTime *t);

// void ISR_ATTR Alarm2_ISR()
void Alarm2_ISR()
{
    // since this interupted any other running code,
    // don't do anything that takes long and especially avoid
    // any communications calls within this routine
    alarm_goes = true;
}

void Button3_ISR()
{
  alarm_goes = true;
}


void setup() {
// int main() {
  // delay(300u);
  pinMode(RTC_INT_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);

  display_pwr(1);
  delay(300u);
  display_init();
  display_point(0, true);

  Wire.begin();
  display_point(1, true);
  
  display_clear();
  display_set_intensity(MIN_INTENSITY+2);
  display_point(0, true);

  delay(3000u);
  display_point(1, true);

  alarm_goes = false;
  rtc_init();
  display_point(2, true);
  update_time(&time);
  rtc_alarm_every_minute();
  attachInterrupt(digitalPinToInterrupt(RTC_INT_PIN), Alarm2_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON3_PIN), Button3_ISR, FALLING);
  display_point(3, true);
  delay(1000);
  display_point(0, false);
  display_point(1, false);
  display_point(2, false);
  display_point(3, false);
}


void loop() {
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


void update_time(RtcDateTime *t)
{
  rtc_get_time(t);
  display_time(t);
}