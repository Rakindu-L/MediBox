# include "DHTesp.h"
# include "oled.h"

# ifndef DHT22_H
# define DHT22_H

class Dht22
{
    private:
        DHTesp* dht;
        Oled* display;

        int temperature_warn = 0;
        int humidity_warn = 0;

        int pin;
        int led_pin;

    public:
        Dht22(int pin, int led_pin, Oled* display);
        void initDht();
        bool checkTemp();
        int getTemperature();

        TempAndHumidity data;
};

# endif