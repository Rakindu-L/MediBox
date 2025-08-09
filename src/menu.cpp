# include "menu.h"

Menu::Menu() {
  
}

void Menu::initMenu(Oled* display, TimeAndAlarms* time_and_alarms,int menu_pin, int up_pin, int down_pin, int select_pin) {
    this->display = display;
    this->time_and_alarms = time_and_alarms;
    this->menu_pin = menu_pin;
    this->up_pin = up_pin;
    this->down_pin = down_pin;
    this->select_pin = select_pin;
    pinMode(menu_pin, INPUT);
    pinMode(up_pin, INPUT);
    pinMode(down_pin, INPUT);
    pinMode(select_pin, INPUT);

}

void Menu::gotoMenu() {

    display->writeData("Menu", 3, 25, 20);
    delay(1000);

    current_count = 0;
    current_page = 0;
    current_item = 0;
    select = false;

    while(1){

        if(digitalRead(menu_pin) == HIGH) {
            Serial.println("Menu exit");
            display->clearDisplay();
            break;
        } else if(digitalRead(up_pin) == HIGH) {
            Serial.println("up");
            current_count++;
            delay(300);
        } else if(digitalRead(down_pin) == HIGH) {
            Serial.println("down");
            current_count = max(0, current_count - 1);
            delay(300);
        } else if(digitalRead(select_pin) == HIGH) {
            Serial.println("select");
            delay(300);
            select = true;
            current_count = 0;
        }

        updateMenu();
        DisplayMenu();
    }
}

void  Menu::updateMenu() {

    if(current_page == 0){
        if(select){
            current_page = main_menu[current_item].page;
            current_item = 0;
            select = false;
        }
        switch(current_count % 4){
            case 0:
                current_item = 0;
                break;
            case 1:
                current_item = 1;
                break;
            case 2:
                current_item = 2;
                break;
            case 3: 
                current_item = 3;
                break;
            default:
                break;
        }
    }else if(current_page == 1){
        this->setTime();
        if(time_set){
            current_page = 0;
            current_item = 0;
            time_set = false;
            time_and_alarms->setUTC(set_hour, set_minute);
            set_hour = 0;
            set_minute = 0;
        }
    }else if(current_page == 2){
        if(select){
            current_page = set_alarm[current_item].page;
            current_item = 0;
            select = false;
        }
        switch(current_count % 3){
            case 0:
                current_item = 0;
                break;
            case 1:
                current_item = 1;
                break;
            case 2:
                current_item = 2;
                break;
            default:
                break;
        }
    }else if(current_page == 3){
        view_alarms[0].item_name =  time_and_alarms->getAlarm1();
        view_alarms[1].item_name =  time_and_alarms->getAlarm2();
        if(select){
            current_page = view_alarms[current_item].page;
            current_item = 0;
            select = false;
        }
        switch(current_count % 3){
            case 0:
                current_item = 0;
                break;
            case 1:
                current_item = 1;
                break;
            case 2:
                current_item = 2;
                break;
            default:
                break;
        }
    }else if(current_page == 4){
        if(select){
            current_page = disable_alarm[current_item].page;
            current_item = 0;
            select = false;
        }
        switch(current_count % 3){
            case 0:
                current_item = 0;
                break;
            case 1:
                current_item = 1;
                break;
            case 2:
                current_item = 2;
                break;
            default:
                break;
        }
    }else if(current_page == 5){
        this->setTime();
        if(time_set){
            current_page = 0;
            current_item = 0;
            time_set = false;
            time_and_alarms->setAlarm1(set_hour, set_minute);
            display->writeData("Alarm 1 set", 1, 15, 30);
            delay(1000);
            set_hour = 0;
            set_minute = 0;
        }
    }else if(current_page == 6){
        this->setTime();
        if(time_set){
            current_page = 0;
            current_item = 0;
            time_set = false;
            time_and_alarms->setAlarm2(set_hour, set_minute);
            display->writeData("Alarm 2 set", 1, 15, 30);
            delay(1000);
            set_hour = 0;
            set_minute = 0;
        }
    }else if(current_page == 7){
        time_and_alarms->disableAlarm1();
        current_page = 0;
        current_item = 0;
        display->writeData("Alarm 1 disabled", 1, 15, 30);
        delay(1000);
        display->clearDisplay();
    }else if(current_page == 8){
        time_and_alarms->disableAlarm2();
        current_page = 0;
        current_item = 0;
        display->writeData("Alarm 2 disabled", 1, 15, 30);
        delay(1000);
        display->clearDisplay();
    }
}

void Menu::DisplayMenu() {

    int x = 2;
    int y = 15;
    int size = 1;
    if(current_page == 0){
        display->writeData(main_menu[current_item].item_name, size, x, y);
    }else if(current_page == 2){
        display->writeData(set_alarm[current_item].item_name, size, x, y);
    }else if(current_page == 3){
        display->writeData(view_alarms[current_item].item_name, size, x, y);
    }else if(current_page == 4){
        display->writeData(disable_alarm[current_item].item_name, size, x, y);
    }

}

void Menu::setTime() {
    int temp_hour = 0;
    int temp_minute = 0;
    char buffer[100];

    sprintf(buffer, "set time - hh:mm");
    display->writeData(buffer, 1, 15, 30);


    while(1){
        if(digitalRead(menu_pin) == HIGH) {
            Serial.println("exit time");
            display->clearDisplay();
            return;
        } else if(digitalRead(up_pin) == HIGH) {
            Serial.println("up");
            temp_hour = (temp_hour + 1) % 24;
            sprintf(buffer, "set time - %02d:%02d", temp_hour, temp_minute);
            display->writeData(buffer, 1, 15, 30);
            delay(50);
        } else if(digitalRead(down_pin) == HIGH) {
            Serial.println("down");
            temp_hour = max(0, temp_hour - 1);
            sprintf(buffer, "set time - %02d:%02d", temp_hour, temp_minute);
            display->writeData(buffer, 1, 15, 30);
            delay(200);
        } else if(digitalRead(select_pin) == HIGH) {
            Serial.println("select");
            delay(300);
            break;
        }
    }

    while(1){
        if(digitalRead(menu_pin) == HIGH) {
            Serial.println("exit time");
            display->clearDisplay();
            return;
        } else if(digitalRead(up_pin) == HIGH) {
            Serial.println("up");
            temp_minute = (temp_minute + 1)%60;
            sprintf(buffer, "set time - %02d:%02d", temp_hour, temp_minute);
            display->writeData(buffer, 1, 15, 30);
            delay(50);
        } else if(digitalRead(down_pin) == HIGH) {
            Serial.println("down");
            temp_minute = max(0, temp_minute - 1);
            sprintf(buffer, "set time - %02d:%02d", temp_hour, temp_minute);
            display->writeData(buffer, 1, 15, 30);
            delay(300);
        } else if(digitalRead(select_pin) == HIGH) {
            Serial.println("select");
            delay(300);
            break;
        }
    }

    set_hour = temp_hour;
    set_minute = temp_minute;
    time_set = true;
}