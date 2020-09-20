

#ifndef __RTCDATETIME8BIT
#define __RTCDATETIME8BIT

#include <Arduino.h>

const uint16_t c_OriginYear = 2000;

class RtcDateTime8bit
{
private:
    uint8_t _yearFrom2000;
    uint8_t _month;
    uint8_t _dayOfMonth;
    uint8_t _hour;
    uint8_t _minute;
    uint8_t _second;
    
public:
    RtcDateTime8bit(uint16_t year,
        uint8_t month,
        uint8_t dayOfMonth,
        uint8_t hour,
        uint8_t minute,
        uint8_t second) :
        _yearFrom2000((year >= c_OriginYear) ? year - c_OriginYear : year),
        _month(month),
        _dayOfMonth(dayOfMonth),
        _hour(hour),
        _minute(minute),
        _second(second)
    {
    }

    bool IsValid() const;

    uint16_t Year() const;
    uint8_t Month() const;
    uint8_t Day() const;
    uint8_t Hour() const;
    uint8_t Minute() const;
    uint8_t Second() const;

    void setYear(uint16_t y);
    void setMonth(uint8_t m);
    void setDay(uint8_t d);
    void setHour(uint8_t h);
    void setMinute(uint8_t mn);
    void setSecond(uint8_t s);


    // convert our Day of Week to Rtc Day of Week 
    // RTC Hardware Day of Week is 1-7, 1 = Monday
    static uint8_t ConvertDowToRtc(uint8_t dow)
    {
        if (dow == 0)
        {
            dow = 7;
        }
        return dow;
    }

    // convert Rtc Day of Week to our Day of Week
    static uint8_t ConvertRtcToDow(uint8_t rtcDow)
    {
        return (rtcDow % 7);
    }


};






#endif //__RTCDATETIME8BIT