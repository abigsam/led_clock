
#include "max7219.h"

static uint8_t inited = 0u;



void max7219_init()
{
    pinMode(MAX7219_CS_PIN, OUTPUT);
    SPI.begin();
    inited = 1;
}



void max7219_send(max7219_reg_t reg, uint8_t value)
{
    if (inited) {
        digitalWrite(MAX7219_CS_PIN, LOW);
        SPI.transfer(reg);
        SPI.transfer(value);
        digitalWrite(MAX7219_CS_PIN, HIGH);
    }
}


void max7219_cs_pin(uint8_t st)
{
    if (!inited)
        pinMode(MAX7219_CS_PIN, OUTPUT);
    digitalWrite(MAX7219_CS_PIN, (st == 0 ? LOW : HIGH));
}