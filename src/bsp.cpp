
#include "bsp.h"


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
        intens = (lux - LOW_LUX_SET)*(HIGH_LUX_SET - LOW_LUX_SET)/(MAX_INTENSITY - MIN_INTENSITY);
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