#include <WiFi.h>
#include "time.h"

#ifndef TIMEANDALARMS_H
#define TIMEANDALARMS_H

struct Alarm
{
    int alarm_id;
    int alarm_hour;
    int alarm_minute;
    bool alarm_active;
    bool alarm_triggered;
    bool alarm_snoozed;
    bool alarm_stopped;
    unsigned long snooze_act_time;
};


class TimeAndAlarms {
    private: 

        const char* ntpServer = "pool.ntp.org";
        long int gmtOffset_sec = 0;

        Alarm alarm1{1, 0, 0, false, false, false, false, 0};
        Alarm alarm2{2, 0, 0, false, false, false, false, 0};

        int snooze_time = 5; // minutes

        int snooze_button = 0;
        int stop_button = 0;
        int buzz_pin = 0;

        void checkAlarm1();
        void checkAlarm2();

    public:
        int current_hour;
        int current_minute;
        int current_second;

        int current_day;
        int current_month;
        int current_year;

        TimeAndAlarms(int snooze_button, int stop_button, int buzz_pin);
        void updateTime();
        void setUTC(int hour, int minute);

        void setAlarm1(int hour, int minute);
        void setAlarm2(int hour, int minute);
        void disableAlarm1();
        void disableAlarm2();

        String getAlarm1();
        String getAlarm2();

        void checkAlarms();


};

#endif