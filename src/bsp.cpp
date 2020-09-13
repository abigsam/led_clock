
#include "bsp.h"

#define MINUTE_IN_SECONDS           ((uint32_t)60)
#define MINUTE10_IN_SECONDS         (MINUTE_IN_SECONDS*10)
#define HOUR_IN_SECONDS             ((uint32_t)60*MINUTE_IN_SECONDS)
#define HOUR10_IN_SECONDS           (HOUR_IN_SECONDS*10)

RtcDateTime time;

void bsp_init()
{
    pinMode(LSENSOR_PIN, INPUT);
    pinMode(BUTTON_MODE_PIN, INPUT);
    pinMode(BUTTON_PLUS_PIN, INPUT);
    pinMode(BUTTON_MINUS_PIN, INPUT);
}


uint8_t bsp_read_button(button_t btn)
{
    if (BUTTON_MODE == btn)
        return !digitalRead(BUTTON_MODE_PIN);
    if (BUTTON_PLUS == btn)
        return !digitalRead(BUTTON_PLUS_PIN);
    if (BUTTON_MINUS == btn)
        return !digitalRead(BUTTON_MINUS_PIN);
    return 0u;
}


void bsp_set_intens()
{
    uint8_t intens = 0;
    uint16_t lux = bsp_lsensor_read();
    if (lux < LOW_LUX_SET) {
        intens = MIN_INTENSITY;
    } else if (lux > HIGH_LUX_SET) {
        intens = MAX_INTENSITY;
    } else {
        intens = (lux - LOW_LUX_SET)*(MAX_INTENSITY - MIN_INTENSITY)/(HIGH_LUX_SET - LOW_LUX_SET);
    }
    display_set_intensity(intens);
}


/**
 * @brief Read value from Light sensor.
 *        ADC result can be equalt to lux with 3% error
 * 
 * @return uint16_t     Result in lux
 */
uint16_t bsp_lsensor_read(void)
{
    return analogRead(LSENSOR_PIN);
}


/**
 * @brief 
 * 
 */
void bsp_update_time()
{
    rtc_get_time(&time);
    display_time(&time);
}

/**
 * @brief 
 * 
 */
void bsp_edit_time()
{
    uint8_t edit_cnt = 0u;
    uint8_t curr_second = 0u;
    uint8_t upd_confirm = 0u;
    uint8_t blink_cnt = 0;

    //Clear seconds in RTC time
    rtc_get_time(&time);
    curr_second = time.Second();
    if (curr_second) //Clear seconds
        time -= curr_second;
    
    display_point(1, true); //Decimal point always on

    //Edit hours
    upd_confirm = 0u;
    display_time(&time, ((1 << 0) | (1 << 1))); //Show only hours
    do {
        delay(200u);
        edit_cnt++;
        blink_cnt++;

        if (blink_cnt%2) {
            if (bsp_read_button(BUTTON_PLUS)) {
                if (time.Hour() == 23)
                    time -= ((uint32_t)23)*HOUR_IN_SECONDS;
                else
                    time += HOUR_IN_SECONDS;
                edit_cnt = 0u;
            } else if (bsp_read_button(BUTTON_MINUS)) {
                if (time.Hour() == 0)
                    time += ((uint32_t)23)*HOUR_IN_SECONDS;
                else
                    time -= HOUR_IN_SECONDS;
                edit_cnt = 0u;
            }
            //Display changes
            display_time(&time, ((1 << 0) | (1 << 1)));
        } else {
            display_time(&time, 0u);
        }

        if (bsp_read_button(BUTTON_MODE)) {
            upd_confirm++;
            break;
        }
    } while(edit_cnt < 70u);

    if (!upd_confirm)
        goto UPD_TIME;

    //Edit minutes
    upd_confirm = 0u;
    edit_cnt = 0;
    blink_cnt = 0;
    display_time(&time, ((1 << 2) | (1 << 3)));
    do {
        delay(200u);
        edit_cnt++;
        blink_cnt++;

        if (blink_cnt%2) {
            if (bsp_read_button(BUTTON_PLUS)) {
                if (time.Minute() == 59u)
                    time -= HOUR_IN_SECONDS - MINUTE_IN_SECONDS; //-59 minutes
                else
                    time += MINUTE_IN_SECONDS;
                edit_cnt = 0u;
            } else if (bsp_read_button(BUTTON_MINUS)) {
                if (time.Minute() == 0u)
                    time += HOUR_IN_SECONDS - MINUTE_IN_SECONDS; //+59 minutes
                else
                    time -= MINUTE_IN_SECONDS;
                edit_cnt = 0u;
            }
            //Display changes
            display_time(&time, ((1 << 2) | (1 << 3)));
        } else {
            display_time(&time, 0u);
        }
        
        if (bsp_read_button(BUTTON_MODE)) {
            upd_confirm++;
            break;
        }
    } while(edit_cnt < 70u);

    //Write new time to the external RTC with cleared seconds
UPD_TIME:
    rtc_set_time(&time);
    delay(100);
    bsp_update_time();
}