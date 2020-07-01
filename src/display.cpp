
#include <avr/pgmspace.h>
#include "display.h"


#define DP_REGISTER         ((max7219_reg_t)3u)


// const PROGMEM uint8_t charSet[] = {
const uint8_t charSet[] = {
//    DFGAEpCB
    0b11011011, //dig "0"
    0b00000011, //dig "1"
    0b10111001, //dig "2"
    0b10110011, //dig "3"
    0b01100011, //dig "4"
    0b11110010, //dig "5"
    0b11111010, //dig "6"
    0b00010011, //dig "7"
    0b11111011, //dig "8"
    0b11110011, //dig "9"
    0b00100000  // "-"
};

const uint8_t digitMask[DISPLAY_DIGITS] = {
    (1u << 3),
    (1u << 7),
    (1u << 1),
    (1u << 6)
};

// const uint8_t segMask[8] = {
//     (1u << 0), //B
//     (1u << 1), //C
//     (1u << 2), //dp
//     (1u << 3), //E
//     (1u << 4), //A
//     (1u << 5), //G
//     (1u << 6), //F
//     (1u << 7)  //D
// };


static uint8_t inited = 0u;
static uint8_t dg[DISPLAY_DIGITS] = {0u};
static uint8_t dg_points = 0u;

static void update_display();
static uint8_t convert_char(char *c);

static void update_display()
{
    uint8_t reg = 0u, seg_cnt = 0u, digit_num = 0u;
    for(seg_cnt = FIRST_DIGIT; seg_cnt <= LAST_DIGIT; seg_cnt++) {
        if (seg_cnt != DP_REGISTER) {
            reg = 0u;
            for(digit_num = 0u; digit_num < DISPLAY_DIGITS; digit_num++) {
                // if (dg[digit_num] & segMask[seg_cnt-1])
                if (dg[digit_num] & (1u << (seg_cnt-1u)))
                    reg |= digitMask[digit_num];
            }
            max7219_send((max7219_reg_t)seg_cnt, reg);
        }
    }
}


static uint8_t convert_char(char *c)
{
    if (*c >= '0' || *c <= '9')
        return charSet[(*c)-'0'];
    else
        return charSet[10];
}


void display_pwr(uint8_t st_enabled)
{
    if (st_enabled) {
        if (!inited)
            pinMode(9, OUTPUT);
        digitalWrite(9, HIGH);
        max7219_cs_pin(1);
    } else {
        if (!inited)
            pinMode(9, OUTPUT);
        digitalWrite(9, LOW);
        max7219_cs_pin(0);
    }
}


void display_init()
{
    display_pwr(1);
    max7219_init();
    max7219_send(DECODE_MODE, 0u);
    max7219_send(SCAN_LIMIT, 7u);
    max7219_send(SHUTDOWN, 1);
    display_clear();
    display_set_intensity(DEFAULT_INTENSITY);
    for(uint8_t cnt = 0; cnt < DISPLAY_DIGITS; cnt++) {
        dg[cnt] = 0u;
    }
    dg_points = 0u;
    inited = 1;
}


void display_shutdown(uint8_t en)
{
    if (inited) {
        if(en) {
            max7219_send(SHUTDOWN, 0);
            digitalWrite(9, LOW);
            max7219_cs_pin(0);
        } else {
            max7219_cs_pin(1);
            digitalWrite(9, HIGH);
            max7219_send(SHUTDOWN, 1);
        }
    }
}


void display_clear()
{
    for(uint8_t cnt = FIRST_DIGIT; cnt <= LAST_DIGIT; cnt++) {
        max7219_send((max7219_reg_t)cnt, 0u);
    }
}


void display_set_intensity(uint8_t value)
{
    if (value > MAX_INTENSITY)
        value = MAX_INTENSITY;
    max7219_send(INTENSITY, value);
}


void display_set(char *str)
{
    char *ptr;
    for(uint8_t cnt = 0u; cnt < DISPLAY_DIGITS; cnt++) {
        ptr = str+cnt;
        if (*ptr != NULL) {
            dg[cnt] = convert_char(ptr);
        }
    }
    update_display();
}


void display_point(uint8_t num, bool state)
{
    if (num < DISPLAY_DIGITS) {
        if (state)
            dg_points |= digitMask[num];
        else
            dg_points &= ~digitMask[num];
    }
    max7219_send(DP_REGISTER, dg_points);
}