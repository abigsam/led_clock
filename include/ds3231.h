
#ifndef __DS3231_H
#define __DS3231_H

#include <Arduino.h>
#include "rtcdatetime8bit.h"
#include <RtcDS3231.h>


class DS3231
{
private:
    uint8_t _lastError;
    uint8_t getReg(uint8_t regAddress);
    void setReg(uint8_t regAddress, uint8_t regValue);

public:
    DS3231() {};
    void Begin() {};

    bool IsDateTimeValid();
    uint8_t LastError();
    bool GetIsRunning();
    void SetIsRunning(bool isRunning);
    void Enable32kHzPin(bool enable);
    void SetSquareWavePin(DS3231SquareWavePinMode pinMode, bool enableWhileInBatteryBackup = true);
    void SetSquareWavePinDisabled();
    void SetAlarmTwo(const DS3231AlarmTwo& alarm);
    DS3231AlarmFlag LatchAlarmsTriggeredFlags();
    void SetDateTime(RtcDateTime8bit *dt);
    void GetDataTime(RtcDateTime8bit *dt);
};





#endif //__DS3231_H






// SetDateTime

//GetDateTime
