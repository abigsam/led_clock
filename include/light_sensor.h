



#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

#include <Arduino.h>

#define LSENSOR_PIN             (A3)  //OR PC3/ADC3

// typedef enum {
//     LSENSOR_RAW = 0,
//     LSENSOR_LUX
// } lsensor_data_t;


void lsensor_init(void);
uint8_t lsensor_read(void);//lsensor_data_t type);


#endif //__LIGHT_SENSOR_H