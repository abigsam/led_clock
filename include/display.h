


#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <Arduino.h>
#include <max7219.h>

#define DSPL_EN_PIN             (9)
#define DEFAULT_INTENSITY       ((uint8_t)3u)
#define MAX_INTENSITY           ((uint8_t)15)
#define DISPLAY_DIGITS          (4)

void display_pwr(uint8_t st_enabled);
void display_init();
void display_shutdown(uint8_t en);
void display_clear();
void display_set_intensity(uint8_t value);
void display_set(char *str);
void display_point(uint8_t num, bool state);

#endif //__DISPLAY_H