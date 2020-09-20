
#include "rtcdatetime8bit.h"



bool RtcDateTime8bit::IsValid() const
{
    // this just tests the most basic validity of the value ranges
    // and valid leap years
    // It does not check any time zone or daylight savings time
    if ((_month > 0 && _month < 13) &&
        (_dayOfMonth > 0 && _dayOfMonth < 32) &&
        (_hour < 24) &&
        (_minute < 60) &&
        (_second < 60))
    {
        // days in a month tests
        //
        if (_month == 2)
        {
            if (_dayOfMonth > 29)
            {
                return false;
            }
            else if (_dayOfMonth > 28)
            {
                // leap day
                // check year to make sure its a leap year
                uint16_t year = Year();

                if ((year % 4) != 0)
                {
                    return false;
                }

                if ((year % 100) == 0 &&
                    (year % 400) != 0)
                {
                    return false;
                }
            }
        }
        else if (_dayOfMonth == 31)
        {
            if ((((_month - 1) % 7) % 2) == 1)
            {
                return false;
            }
        }

        return true;
    }
    return false;
}


uint16_t RtcDateTime8bit::Year() const
{
    return c_OriginYear + _yearFrom2000;
}

uint8_t RtcDateTime8bit::Month() const
{
    return _month;
}

uint8_t RtcDateTime8bit::Day() const
{
    return _dayOfMonth;
}

uint8_t RtcDateTime8bit::Hour() const
{
    return _hour;
}

uint8_t RtcDateTime8bit::Minute() const
{
    return _minute;
}

uint8_t RtcDateTime8bit::Second() const
{
    return _second;
}

void RtcDateTime8bit::setYear(uint16_t y)
{
    _yearFrom2000 = c_OriginYear - y;
}

void RtcDateTime8bit::setMonth(uint8_t m)
{
    _month = m;
}

void RtcDateTime8bit::setDay(uint8_t d)
{
    _dayOfMonth = d;
}

void RtcDateTime8bit::setHour(uint8_t h)
{
    _hour = h;
}

void RtcDateTime8bit::setMinute(uint8_t mn)
{
    _minute = mn;
}

void RtcDateTime8bit::setSecond(uint8_t s)
{
    _second = s;
}