
#include "ds3231.h"


uint8_t DS3231::getReg(uint8_t regAddress)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(regAddress);
    _lastError = Wire.endTransmission();
    if (_lastError != 0)
    {
        return 0;
    }

    // control register
    uint8_t bytesRead = Wire.requestFrom(DS3231_ADDRESS, (uint8_t)1);
    if (1 != bytesRead)
    {
        _lastError = 4;
        return 0;
    }

    uint8_t regValue = Wire.read();
    return regValue;
}


void DS3231::setReg(uint8_t regAddress, uint8_t regValue)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(regAddress);
    Wire.write(regValue);
    _lastError = Wire.endTransmission();
}

bool DS3231::IsDateTimeValid()
{
    uint8_t status = getReg(DS3231_REG_STATUS);
    return (!(status & _BV(DS3231_OSF)) && (_lastError == 0));
}


uint8_t DS3231::LastError()
{
    return _lastError;
}


bool DS3231::GetIsRunning()
{
    uint8_t creg = getReg(DS3231_REG_CONTROL);
    return (!(creg & _BV(DS3231_EOSC)) && (_lastError == 0));
}


void DS3231::SetIsRunning(bool isRunning)
{
    uint8_t creg = getReg(DS3231_REG_CONTROL);
    if (isRunning)
    {
        creg &= ~_BV(DS3231_EOSC);
    }
    else
    {
        creg |= _BV(DS3231_EOSC);
    }
    setReg(DS3231_REG_CONTROL, creg);
}


void DS3231::Enable32kHzPin(bool enable)
{
    uint8_t sreg = getReg(DS3231_REG_STATUS);

    if (enable == true)
    {
        sreg |= _BV(DS3231_EN32KHZ);
    }
    else
    {
        sreg &= ~_BV(DS3231_EN32KHZ);
    }

    setReg(DS3231_REG_STATUS, sreg);
}


void DS3231::SetSquareWavePin(DS3231SquareWavePinMode pinMode, bool enableWhileInBatteryBackup)
{
    uint8_t creg = getReg(DS3231_REG_CONTROL);

    // clear all relevant bits to a known "off" state
    creg &= ~(DS3231_AIEMASK | _BV(DS3231_BBSQW));
    creg |= _BV(DS3231_INTCN);  // set INTCN to disables clock SQW

    if (pinMode != DS3231SquareWavePin_ModeNone)
    {
        if (pinMode == DS3231SquareWavePin_ModeClock)
        {
            creg &= ~_BV(DS3231_INTCN); // clear INTCN to enable clock SQW 
        }
        else
        {
            if (pinMode & DS3231SquareWavePin_ModeAlarmOne)
            {
                creg |= _BV(DS3231_A1IE);
            }
            if (pinMode & DS3231SquareWavePin_ModeAlarmTwo)
            {
                creg |= _BV(DS3231_A2IE);
            }
        }

        if (enableWhileInBatteryBackup)
        {
            creg |= _BV(DS3231_BBSQW); // set enable int/sqw while in battery backup flag
        }
    }
    setReg(DS3231_REG_CONTROL, creg);
}


void DS3231::SetSquareWavePinDisabled()
{
    uint8_t creg = getReg(DS3231_REG_CONTROL);

    // clear all relevant bits to a known "off" state
    creg &= ~(DS3231_AIEMASK | _BV(DS3231_BBSQW));
    creg |= _BV(DS3231_INTCN);  // set INTCN to disables clock SQW
    setReg(DS3231_REG_CONTROL, creg);
}


void DS3231::SetAlarmTwo(const DS3231AlarmTwo& alarm)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(DS3231_REG_ALARMTWO);

    Wire.write(Uint8ToBcd(alarm.Minute()) | ((alarm.ControlFlags() & 0x01) << 7));
    Wire.write(Uint8ToBcd(alarm.Hour()) | ((alarm.ControlFlags() & 0x02) << 6)); // 24 hour mode only

    // convert our Day of Week to Rtc Day of Week if needed
    uint8_t rtcDow = alarm.DayOf();
    if (alarm.ControlFlags() == DS3231AlarmTwoControl_HoursMinutesDayOfWeekMatch)
    {
        rtcDow = RtcDateTime::ConvertDowToRtc(rtcDow);
    }
    
    Wire.write(Uint8ToBcd(rtcDow) | ((alarm.ControlFlags() & 0x0c) << 4));

    _lastError = Wire.endTransmission();
}


DS3231AlarmFlag DS3231::LatchAlarmsTriggeredFlags()
{
    uint8_t sreg = getReg(DS3231_REG_STATUS);
    uint8_t alarmFlags = (sreg & DS3231_AIFMASK);
    sreg &= ~DS3231_AIFMASK; // clear the flags
    setReg(DS3231_REG_STATUS, sreg);
    return (DS3231AlarmFlag)alarmFlags;
}


void DS3231::SetDateTime(RtcDateTime8bit *dt)
{

}


void DS3231::GetDataTime(RtcDateTime8bit *dt)
{

}



