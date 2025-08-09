# include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "oled.h"
#include "timeAndAlarms.h"

#ifndef MENU_H
#define MENU_H

struct menu_item
{
    String item_name;
    int item_id;
    int no_of_children;
    int page;
    int parent;
};


class Menu {
  public:
  
    Menu();
    void initMenu(Oled* display, TimeAndAlarms* time_and_alarms, int menu_pin, int up_pin, int down_pin, int select_pin);
    void updateMenu();
    void gotoMenu();

  private:

    Oled* display;
    TimeAndAlarms* time_and_alarms;

    void DisplayMenu();

    int set_hour = 0;
    int set_minute = 0;
    bool time_set = false;

    int menu_pin;
    int up_pin;
    int down_pin;
    int select_pin;

    bool select = false;

    int current_page = 0;
    int current_item = 0;
    int current_count = 0;


    void setTime();

    menu_item main_menu[4] = { // page 0
        {"set UTC", 0, 0, 1, 0},
        {"set Alarm", 1, 2, 2, 0},
        {"view Alarms", 2, 0, 3, 0},
        {"disable Alarm", 3, 2, 4, 0},
    };

    menu_item set_utc[1] = { // page 1
        {"back", 0, 0, 0, 1},
    };

    menu_item set_alarm[3] = { // page 2
        {"set Alarm 1", 0, 0, 5, 2},
        {"set Alarm 2", 1, 0, 6, 2},
        {"back", 2, 0, 0, 2},
    };


    menu_item view_alarms[3] = { // page 3
        {"", 0, 0, 3, 3},
        {"", 1, 0, 3, 3},
        {"back", 2, 0, 0, 3},
    };

    menu_item disable_alarm[3] = { // page 4
        {"disable Alarm 1", 0, 0, 7, 4},
        {"disable Alarm 2", 1, 0, 8, 4},
        {"back", 2, 0, 0, 4},

    };

    // menu_item set_alarm_1[1] = { // page 5
    //     {"back", 0, 0, 2, 5},
    // };

    // menu_item set_alarm_2[1] = { // page 6
    //     {"back", 0, 0, 2, 6},
    // };

    menu_item disable_alarm_1[1] = { // page 7
        {"back", 0, 0, 4, 7},
    };

    menu_item disable_alarm_2[1] = { // page 8
        {"back", 0, 0, 4, 8},
    };
    
};

#endif