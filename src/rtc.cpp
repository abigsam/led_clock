
#include "rtc.h"

static RtcDS3231<TwoWire> rtc(Wire);
// // EepromAt24c32<TwoWire> eeprom(Wire);


void rtc_init()
{
    rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    // // display_dbg(1);
    if (!rtc.IsDateTimeValid()) 
    {
        if (rtc.LastError() != 0)
        {
    //         // we have a communications error
    //         // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
    //         // what the number means
    //         // Serial.print("RTC communications error = ");
    //         // Serial.println(Rtc.LastError());
    //         display_dbg(2);
        }
        else
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            // Serial.println("RTC lost confidence in the DateTime!");

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            rtc.SetDateTime(compiled);
    //         display_dbg(3);
        }
    }

    if (!rtc.GetIsRunning())
    {
    //     // Serial.println("RTC was not actively running, starting now");
        rtc.SetIsRunning(true);
    //     // display_dbg(4);
    }

    // display_dbg(5);



    // // display_dbg(1);
    rtc.Enable32kHzPin(false);
    // // display_dbg(2);
    rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
    
    // // display_dbg(3);
    // if (!rtc.GetIsRunning())
    // {
    //     rtc.SetIsRunning(true);
    //     RtcDateTime now = rtc.GetDateTime();
    //     if (now < compiled) 
    //     {
    //         rtc.SetDateTime(compiled);
    //     }
    // }
    // // display_dbg(4);
}


void rtc_get_time(RtcDateTime *t_get)
{
    *t_get = rtc.GetDateTime();
}


void rtc_set_time(RtcDateTime *t_set)
{
    rtc.SetDateTime(*t_set);
}


void rtc_get_temperature(int16_t *tmpr)
{
    RtcTemperature temp = rtc.GetTemperature();
    *tmpr = temp.AsCentiDegC(); //centi degrees (1/100th of a degree)
}


void rtc_alarm_every_minute()
{
    DS3231AlarmTwo alarm2(0, 0, 0,  DS3231AlarmTwoControl_OncePerMinute);
    rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
    rtc.SetAlarmTwo(alarm2);
    // throw away any old alarm state before we ran
    rtc.LatchAlarmsTriggeredFlags();
}


bool rtc_alarm2_check()
{
    DS3231AlarmFlag flag = rtc.LatchAlarmsTriggeredFlags();
    return (flag & DS3231AlarmFlag_Alarm2);
}

