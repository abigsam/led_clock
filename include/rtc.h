


#ifndef __RTC_H
#define __RTC_H

#include <Arduino.h>
#include <Wire.h>
#include <RtcDS3231.h>


void rtc_init();
void rtc_get_time(RtcDateTime *t_get);
void rtc_set_time(RtcDateTime *t_set);
void rtc_get_temperature(int16_t *tmpr);
void rtc_set_mem(uint8_t start, uint8_t *ptr, uint8_t size);
void rtc_get_mem(uint8_t start, uint8_t *ptr, uint8_t size);

#endif //__RTC_H