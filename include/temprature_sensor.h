




#ifndef __TEMPERATURE_SENSOR_H
#define __TEMPERATURE_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Temperature_LM75_Derived.h>


void tsensor_init();
void tsensor_get(int16_t *tmpr);





#endif //__TEMPERATURE_SENSOR_H