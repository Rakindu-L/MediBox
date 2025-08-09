#include "oled.h"

Oled::Oled(Adafruit_SSD1306* display, TimeAndAlarms* timeAndAlarms) {
    this->display = display;
    this->timeAndAlarms = timeAndAlarms;
}

void Oled::initOled(float* temp, float* hum) {
    int count = 0;
    while(!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed...retrying"));
        delay(500);
        count++;
        if(count > 3) {
            Serial.println(F("SSD1306 allocation failed"));
            for(;;);
        }
    }
    this->temp = temp;
    this->hum = hum;
    display->display();
    delay(1500);
    display->clearDisplay();
    display->setTextSize(3);
    display->setTextColor(WHITE);

}

void Oled::writeData(String data, int size, int x, int y, bool clear) {
    if(data == prv_data && prv_data != "") {
        return;
    }else{
        prv_data = data;
        if(clear) {
            display->clearDisplay();
        }
        display->setCursor(5, 3);
        display->setTextSize(1);
        
        char buffer[100];
        sprintf(buffer, "%02d/%02d/%d", timeAndAlarms->current_day, timeAndAlarms->current_month, timeAndAlarms->current_year);
        display->print(buffer);

        display->setCursor(90, 3);
        sprintf(buffer, "%02d:%02d", timeAndAlarms->current_hour, timeAndAlarms->current_minute);
        display->print(buffer);

        display->drawLine(0, 13, 128, 13, WHITE);

        display->setCursor(x, y);
        display->setTextSize(size);
        display->println(data);
        display->display();
    }
}

void Oled::clearDisplay() {
    this->writeData("", 0, 20, 0);
}

void Oled::displayTime() {
    display->clearDisplay();
    char buffer[100];
    sprintf(buffer, "%02d:%02d:%02d", timeAndAlarms->current_hour, timeAndAlarms->current_minute, timeAndAlarms->current_second);
    display->setCursor(18, 25);
    display->setTextSize(2);
    display->println(buffer);
    sprintf(buffer, "%02d/%02d/%d", timeAndAlarms->current_day, timeAndAlarms->current_month, timeAndAlarms->current_year);
    display->setCursor(5, 3);
    display->setTextSize(1);
    display->print(buffer);
    display->drawLine(0, 13, 128, 13, WHITE);
    display->setCursor(2, 55);
    display->setTextSize(1);
    sprintf(buffer, "T: %.1fC H: %.1f%", *temp, *hum);
    display->println(buffer);
    display->display();
}