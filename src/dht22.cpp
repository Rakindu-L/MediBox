#include "dht22.h"

Dht22::Dht22(int pin, int led_pin, Oled* display)
{
    this->pin = pin;
    this->led_pin = led_pin;
    this->dht = new DHTesp();
    this->display = display;
}

void Dht22::initDht()
{
    dht->setup(pin, DHTesp::DHT22);
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, LOW);
}

int Dht22::getTemperature()
{
    return data.temperature;
}

bool Dht22::checkTemp()
{
    data = dht->getTempAndHumidity();
    bool all_good = true;

    temperature_warn = 0;
    humidity_warn = 0;

    

    if(data.temperature > 35.0){
        all_good = false;
        digitalWrite(led_pin, HIGH);
        temperature_warn = 1;
    }else if(data.temperature < 25.0){
        all_good = false;
        digitalWrite(led_pin, HIGH);
        temperature_warn = -1;
    }

    if(data.humidity > 85.0){
        all_good = false;
        digitalWrite(led_pin, HIGH);
        humidity_warn = 1;
    }else if(data.humidity < 45.0){
        all_good = false;
        digitalWrite(led_pin, HIGH);
        humidity_warn = -1;
    }
    if(all_good){
        digitalWrite(led_pin, LOW);
    }else{
        char buffer[100];
        if(temperature_warn == 1){
            if(humidity_warn == 1){
                sprintf(buffer, " HIGH TEMP\n HUMD HIGH");
            }else if(humidity_warn == -1){
                sprintf(buffer, " HIGH TEMP\n HUMD LOW");
            }else{
                sprintf(buffer, " HIGH TEMP");
            }
        }else if(temperature_warn == -1){
            if(humidity_warn == 1){
                sprintf(buffer, " LOW TEMP\n HUMD HIGH");
            }else if(humidity_warn == -1){
                sprintf(buffer, " LOW TEMP\n HUMD LOW");
            }else{
                sprintf(buffer, " LOW TEMP");
            }
        }else{
            if(humidity_warn == 1){
                sprintf(buffer, " HUMD HIGH");
            }else if(humidity_warn == -1){
                sprintf(buffer, "HUMD LOW");
            }
        }
        display->writeData(buffer, 1, 0, 30);
    }
    return all_good;
}