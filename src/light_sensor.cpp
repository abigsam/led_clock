

#include "light_sensor.h"


/**
 * @brief Initilize Light sensor
 * 
 */
void lsensor_init(void)
{
    pinMode(LSENSOR_PIN, INPUT);
}


/**
 * @brief Read value from Light sensor.
 *        ADC result can be equalt to lux with 3% error
 * 
 * @return uint8_t  Result in lux
 */
uint8_t lsensor_read(void)
{
    return analogRead(LSENSOR_PIN);
}