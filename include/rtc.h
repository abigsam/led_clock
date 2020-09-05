


#ifndef __RTC_H
#define __RTC_H

#include <Arduino.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include "display.h"


void rtc_init();
void rtc_get_time(RtcDateTime *t_get);
void rtc_set_time(RtcDateTime *t_set);
void rtc_get_temperature(int16_t *tmpr);
void rtc_alarm_every_minute();
bool rtc_alarm2_check();

#endif //__RTC_H