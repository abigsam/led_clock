



#ifndef __GPS_H
#define __GPS_H

#include <Arduino.h>
#include <RtcDS3231.h>

//void gps_init();
void gps_set_shutdown(bool state);
void gps_get_time(RtcDateTime *t_get);
bool gps_is_ready();

#endif //__GPS_H