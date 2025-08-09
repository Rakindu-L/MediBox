#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <menu.h>
#include <timeAndAlarms.h>
#include <WiFi.h>
#include "dht22.h"
#include <PubSubClient.h>
#include <math.h>  
#include <ESP32Servo.h>


const char* mqtt_server = "broker.hivemq.com";

const char* topic_ts = "medibox/config/ts443";
const char* topic_tu = "medibox/config/tu443";

const char* topic_theta = "medibox/config/theta443";
const char* topic_y = "medibox/config/y443";
const char* topic_itemp = "medibox/config/itemp443";
const char* topic_ldr = "medibox/ldr443";
const char* topic_dht = "medibox/dht443";

void reconnect();
void callback(char* topic, byte* message, unsigned int length);
float readLDR();
int calculateServoAngle(float I, float ts, float tu, float T, float theta_offset, float gamma, float Tmed);

int ts = 5; // default sampling interval
int tu = 120; // default time constant
int theta_offset = 30; // default angle offset
float y = 0.75; // default gamma
int Tmed = 30; // default Tmed


unsigned int ldr_count = 0;
float total_intensity = 0;
float average_intensity = 0;

WiFiClient espClient;
PubSubClient client(espClient);
Servo myServo;

int servoPin = 25;

unsigned long lastSampleTime = 0;
unsigned long lastSentTime = 0;
unsigned long lastServoTime = 0;

const char* ssid = "Wokwi-GUEST";
const char* password = ""; 

bool display_time = true;

unsigned long last_time_update = millis();

Menu menu;
unsigned long presstime = millis();

Adafruit_SSD1306 display(128, 64, &Wire, -1);
TimeAndAlarms timeAndAlarms(26, 27, 32);
Oled oled(&display, &timeAndAlarms);
Dht22 dht22(15, 23, &oled);



void setup() {
  Serial.begin(9600);
    pinMode(33, INPUT); 

  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(50);
      Serial.print(".");
  }
  Serial.println(" Connected!");

  client.setServer(mqtt_server, 1883);
  Serial.println("Connected to MQTT broker");
  client.setCallback(callback);
  Serial.println("Callback set");

  myServo.attach(servoPin);
  myServo.write(0);

  timeAndAlarms.updateTime();

  Serial.println("Hello World!");
  oled.initOled(&dht22.data.temperature, &dht22.data.humidity);
  menu.initMenu(&oled, &timeAndAlarms, 12, 14, 27, 26);
  dht22.initDht();
  readLDR();
}

void loop() {
  if(digitalRead(12) == HIGH && millis() - presstime > 700) {
    menu.gotoMenu();
    presstime = millis();
  }

  if(millis() - last_time_update > 500 && display_time) {
    timeAndAlarms.updateTime();
    last_time_update = millis();
    oled.displayTime();
  }

  if(!dht22.checkTemp()) {
    display_time = false;
  }else{
    display_time = true;
  }

  timeAndAlarms.checkAlarms();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (millis() - lastSampleTime >= ts * 1000) {
    lastSampleTime = millis();

    float intensity = 1 - readLDR();
    total_intensity = intensity + total_intensity;
    ldr_count++;
    average_intensity = total_intensity / ldr_count;
  }
  if(millis() - lastSentTime >= 1000 * tu) {
      lastSentTime = millis();
      average_intensity = total_intensity / ldr_count;
      total_intensity = 0;
      ldr_count = 0;
      char buf[8];
      sprintf(buf, "%.2f", average_intensity);
      client.publish(topic_ldr, buf);
      Serial.print("Average LDR Intensity: ");
      Serial.println(buf);
  }

  if(millis() - lastServoTime >= 500) {
    float T = dht22.getTemperature();
    float I = average_intensity;
    int servoAngle = calculateServoAngle(I, ts, tu, T, theta_offset, y, Tmed);
    myServo.write(servoAngle);
    lastServoTime = millis();
    Serial.print("Servo Angle: ");
    Serial.println(servoAngle);
  }
}

int calculateServoAngle(float I, float ts, float tu, float T, float theta_offset, float gamma, float Tmed) {
  if (ts <= 0 || tu <= 0 || Tmed == 0) {
    return theta_offset; 
  }

  float lnRatio = log(ts / tu);
  float theta = theta_offset + (180.0 - theta_offset) * I * gamma * lnRatio * (T / Tmed);

  theta = constrain(theta, 0, 180);

  return (int)theta;
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  if (String(topic) == topic_ts) {
    ts = messageTemp.toInt();
    Serial.print("Updated ts: ");
    Serial.println(ts);
  }
  else if (String(topic) == topic_tu) {
    tu = messageTemp.toInt();
    Serial.print("Updated tu: ");
    Serial.println(tu);
  }
  else if (String(topic) == topic_theta) 
  {
    theta_offset = messageTemp.toInt();
    Serial.print("Updated theta: ");
    Serial.println(theta_offset);
  }
  else if (String(topic) == topic_y) {
    y = messageTemp.toFloat();
    Serial.print("Updated y: ");
    Serial.println(y);
  }
  else if (String(topic) == topic_itemp) {
    Tmed = messageTemp.toInt();
    Serial.print("Updated itemp: ");
    Serial.println(Tmed);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client202")) {
      Serial.println("connected");
      client.subscribe(topic_ts);
      client.subscribe(topic_tu); 
      client.subscribe(topic_theta);
      client.subscribe(topic_y); 
      client.subscribe(topic_itemp); 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

float readLDR() {
  int raw = analogRead(33);
  return raw / 4095.0;
}
