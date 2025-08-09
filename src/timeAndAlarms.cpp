# include "timeAndAlarms.h"



TimeAndAlarms::TimeAndAlarms(int snooze_button, int stop_button, int buzz_pin)
{
    this->snooze_button = snooze_button;
    this->stop_button = stop_button;
    this->buzz_pin = buzz_pin;

    pinMode(buzz_pin, OUTPUT);
}


void TimeAndAlarms::updateTime()
{
    configTime(gmtOffset_sec, 0, ntpServer);
    struct tm timeinfo;
    getLocalTime(&timeinfo);

    char day_str[8];
    char hour_str[8];
    char minute_str[8];
    char second_str[8];
    char year_str[8];


    strftime(day_str, 8, "%d", &timeinfo);
    strftime(hour_str, 8, "%H", &timeinfo);
    strftime(minute_str, 8, "%M", &timeinfo);
    strftime(second_str, 8, "%S", &timeinfo);
    strftime(year_str, 8, "%Y", &timeinfo);

    current_day = atoi(day_str);
    current_hour = atoi(hour_str);
    current_minute = atoi(minute_str);
    current_second = atoi(second_str);
    current_year = atoi(year_str);
    current_month = timeinfo.tm_mon + 1;

}

void TimeAndAlarms::setUTC(int hour, int minute)
{
    gmtOffset_sec = hour*3600 + minute*60;
    updateTime();
}

void TimeAndAlarms::setAlarm1(int hour, int minute)
{
    alarm1.alarm_hour = hour;
    alarm1.alarm_minute = minute;
    alarm1.alarm_active = true;
}

void TimeAndAlarms::setAlarm2(int hour, int minute)
{
    alarm2.alarm_hour = hour;
    alarm2.alarm_minute = minute;
    alarm2.alarm_active = true;
}

void TimeAndAlarms::disableAlarm1()
{
    alarm1.alarm_active = false;
}

void TimeAndAlarms::disableAlarm2()
{
    alarm2.alarm_active = false;
}

String TimeAndAlarms::getAlarm1()
{
    char buffer[100];
    sprintf(buffer, "1 > %02d:%02d - %s", alarm1.alarm_hour, alarm1.alarm_minute, alarm1.alarm_active ? "active" : "inactive");
    return String(buffer);
}

String TimeAndAlarms::getAlarm2()
{
    char buffer[100];
    sprintf(buffer, "2 > %02d:%02d - %s", alarm2.alarm_hour, alarm2.alarm_minute, alarm2.alarm_active ? "active" : "inactive");
    return String(buffer);
}

void TimeAndAlarms::checkAlarms()
{
    checkAlarm1();
    checkAlarm2();
}

void TimeAndAlarms::checkAlarm1()
{
    if(alarm1.alarm_active && current_hour == alarm1.alarm_hour && current_minute == alarm1.alarm_minute)
    {
        if(!alarm1.alarm_triggered && !alarm1.alarm_snoozed && !alarm1.alarm_stopped)
        {
            alarm1.alarm_triggered = true;
        }
        
    }else if(alarm1.alarm_stopped)
    {
        alarm1.alarm_stopped = false;
    }
    if (alarm1.alarm_triggered)
    {
        unsigned int tone_length = 100;
        unsigned long last_buzz;
        int tone_state = 0;
        int notes[] = {262, 349, 523};

        while(true)
        {
            
            if(digitalRead(snooze_button) == HIGH)
            {
                alarm1.alarm_snoozed = true;
                alarm1.snooze_act_time = millis();
                alarm1.alarm_triggered = false;
                noTone(buzz_pin);
                break;
            }else if(digitalRead(stop_button) == HIGH)
            {
                alarm1.alarm_stopped = true;
                alarm1.alarm_triggered = false;
                noTone(buzz_pin);
                break;
            }
            if(millis() - last_buzz > tone_length)
            {
                last_buzz = millis();
                tone(buzz_pin, notes[tone_state]);
                tone_state = (tone_state + 1) % 3;
            }
        }
    }
    if(alarm1.alarm_snoozed && millis() - alarm1.snooze_act_time > 5*60*1000)
    {
        alarm1.alarm_snoozed = false;
        alarm1.alarm_triggered = true;
    }
}

void TimeAndAlarms::checkAlarm2()
{
    if(alarm2.alarm_active && current_hour == alarm2.alarm_hour && current_minute == alarm2.alarm_minute)
    {
        if(!alarm2.alarm_triggered && !alarm2.alarm_snoozed && !alarm2.alarm_stopped)
        {
            alarm2.alarm_triggered = true;
        }
        
    }else if(alarm2.alarm_stopped)
    {
        alarm2.alarm_stopped = false;
    }
    if (alarm2.alarm_triggered)
    {
        unsigned int tone_length = 100;
        unsigned long last_buzz;
        int tone_state = 0;
        int notes[] = {262, 349, 523};

        while(true)
        {
            
            if(digitalRead(snooze_button) == HIGH)
            {
                alarm2.alarm_snoozed = true;
                alarm2.snooze_act_time = millis();
                alarm2.alarm_triggered = false;
                noTone(buzz_pin);
                break;
            }else if(digitalRead(stop_button) == HIGH)
            {
                alarm2.alarm_stopped = true;
                alarm2.alarm_triggered = false;
                noTone(buzz_pin);
                break;
            }
            if(millis() - last_buzz > tone_length)
            {
                last_buzz = millis();
                tone(buzz_pin, notes[tone_state]);
                tone_state = (tone_state + 1) % 3;
            }
        }
    }
    if(alarm2.alarm_snoozed && millis() - alarm2.snooze_act_time > 5*60*1000)
    {
        alarm2.alarm_snoozed = false;
        alarm2.alarm_triggered = true;
    }
}

