


#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <Arduino.h>
#include <max7219.h>
#include <RtcDS3231.h>

#define DSPL_EN_PIN             (9)
#define DEFAULT_INTENSITY       ((uint8_t)3u)
#define MAX_INTENSITY           ((uint8_t)15)
#define MIN_INTENSITY           ((uint8_t)0)
#define DISPLAY_DIGITS          (4)

void display_pwr(uint8_t st_enabled);
void display_init();
void display_shutdown(uint8_t en);
void display_clear();
void display_set_intensity(uint8_t value);
void display_set(char *str, uint8_t pos = 0u, uint8_t flush = 1u);
void display_set(uint16_t num, uint8_t pnum = 0u, uint8_t flush = 1u);
void display_point(uint8_t num, bool state);
void display_update();
void display_dbg(uint8_t num, uint8_t pos = 0);

void display_time(RtcDateTime *time, uint8_t enable_mask = 0xf);
void display_digit_control(uint8_t enable_mask);

#endif //__DISPLAY_H