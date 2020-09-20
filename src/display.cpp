
// #include <avr/pgmspace.h>
#include "display.h"


#define DP_REGISTER         ((max7219_reg_t)3u)
#define CHAR_DEFIS          (10)
#define CHAR_SPACE          (11)
#define CHAR_T              (12)
#define CHAR_L              (13)
#define CHAR_E              (14)
#define CHAR_DEGREE         (15)


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
    0b00100000, // "-"
    0b00000000, // " "
    0b11101000, // "t"
    0b11001000, // "L"
    0b11111000, // "E"
    0b01110001  // "*" (degree)
};

const uint8_t digitMask[DISPLAY_DIGITS] = {
    (1u << 3),
    (1u << 7),
    (1u << 1),
    (1u << 6)
};


static uint8_t inited = 0u;
static uint8_t dg[DISPLAY_DIGITS] = {0u};
static uint8_t dg_copy[DISPLAY_DIGITS] = {0u};
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
    if (*c >= '0' && *c <= '9')
        return charSet[(*c)-'0'];
    else if ('L' == *c)
        return charSet[CHAR_L];
    else if ('T' == *c)
        return charSet[CHAR_T];
    else if ('E' == *c)
        return charSet[CHAR_E];
    else if ('*' == *c)
        return charSet[CHAR_DEGREE];
    else if (' ' == *c)
        return charSet[CHAR_SPACE];
    else
        return charSet[CHAR_DEFIS];
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
        dg_copy[cnt] = 0u;
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


void display_set(char *str, uint8_t pos, uint8_t flush)
{
    for(; pos < DISPLAY_DIGITS; pos++) {
        if (*str != 0) {
            dg[pos] = convert_char(str);
        }
        str++;
    }
    if (flush)
        update_display();
}


void display_set(uint16_t num, uint8_t pos, uint8_t flush)
{
    uint8_t temp[DISPLAY_DIGITS] = {0u};

    //Convert
    if (num < 9999u) {
        temp[0] = (num/1000u) ? charSet[num/1000u] : charSet[CHAR_SPACE];
        num %= 1000u;
        temp[1] = (num/100u || num/1000u) ? charSet[num/100u] : charSet[CHAR_SPACE];
        num %= 100u;
        temp[2] = (num/10u || num/100u || num/1000u) ? charSet[num/10u] : charSet[CHAR_SPACE];
        num %= 10u;
        temp[3] = charSet[num];
    } else {
        temp[0] = charSet[CHAR_SPACE];
        temp[1] = charSet[CHAR_SPACE];
        temp[2] = charSet[CHAR_SPACE];
        temp[3] = charSet[CHAR_DEFIS];
    }

    //Check position
    if (pos >= DISPLAY_DIGITS)
        return;

    //Copy
    for(uint8_t ncnt = DISPLAY_DIGITS; ncnt > 0u; ncnt--) {
        dg[pos] = temp[ncnt-1];
        if (!pos)
            break;
        pos--;
    }

    //Update
    if (flush)
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


void display_update()
{
    update_display();
}

void display_dbg(uint8_t num, uint8_t pos)
{
    if (num > 9)
        return;
    dg[pos] = charSet[num];
    update_display();
}


/**
 * @brief 
 * 
 * @param time 
 */
void display_time(RtcDateTime *time, uint8_t enable_mask)
{
    uint8_t cnt = 0u;

    uint8_t num = time->Minute();
    dg_copy[3] = charSet[num%10];
    if (num/10 < 9)
        dg_copy[2] = charSet[num/10];
    else
        dg_copy[2] = charSet[CHAR_DEFIS];
    num = time->Hour();
    dg_copy[1] = charSet[num%10];
    if (num/10 < 9) {
        if (0 == num/10)
            dg_copy[0] = charSet[CHAR_SPACE];
        else
            dg_copy[0] = charSet[num/10];
    } else {
        dg_copy[0] = charSet[CHAR_DEFIS];
    }

    for(cnt = 0u; cnt < DISPLAY_DIGITS; cnt++) {
        if (!(enable_mask & (1u << cnt))) {
            dg[cnt] = charSet[CHAR_SPACE];
        } else {
            dg[cnt] = dg_copy[cnt];
        }
    }

    update_display();
}


/**
 * @brief 
 * 
 * @param enable_mask 
 */
void display_digit_control(uint8_t enable_mask)
{
    uint8_t cnt = 0u;
    for(cnt = 0u; cnt < DISPLAY_DIGITS; cnt++) {
        if (!(enable_mask & (1u << cnt))) {
            //dg_copy[cnt] = dg[cnt];
            dg[cnt] = charSet[CHAR_SPACE];
        } else {
            dg[cnt] = dg_copy[cnt];
        }
    }
    update_display();
}

