


#ifndef __MAX7219_H
#define __MAX7219_H

#include <Arduino.h>
#include <SPI.h>

#define MAX7219_CS_PIN          (10)

typedef enum {
    NOP = 0x00,
    FIRST_DIGIT = 0x01,
    LAST_DIGIT = 0x08,
    DECODE_MODE = 0x09,
    INTENSITY = 0x0a,
    SCAN_LIMIT = 0x0b,
    SHUTDOWN = 0x0c,
    DISPLAY_TEST = 0x0f
} max7219_reg_t;

void max7219_init();
void max7219_send(max7219_reg_t reg, uint8_t value);
void max7219_cs_pin(uint8_t st);

#endif //__MAX7219_H