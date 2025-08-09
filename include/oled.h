#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <timeAndAlarms.h>

#ifndef OLED_H
#define OLED_H

class Oled{
    private:
        Adafruit_SSD1306* display;
        TimeAndAlarms* timeAndAlarms;

        float* temp;
        float* hum;

        String prv_data = "";

    public:
        Oled(Adafruit_SSD1306* display, TimeAndAlarms* timeAndAlarms);
        void initOled(float* temp, float* hum);
        void writeData(String data, int size, int x, int y, bool clear = true);
        void clearDisplay();
        void displayTime();
};

#endif