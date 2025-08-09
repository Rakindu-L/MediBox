# Medibox – Smart Medicine Storage & Reminder System  


## 📖 Overview  
The **Medibox** is a smart device designed to help users manage their medication schedules and ensure optimal storage conditions for sensitive medicines.  
This was developed as part of a **university semester project** and builds upon the basic Medibox concept with **light intensity monitoring**, **servo-controlled shading**, and **configurable parameters** via a **Node-RED dashboard**.  

The implementation uses an **ESP32 microcontroller**, **Node-RED** for the UI, and a combination of sensors and actuators for environmental monitoring and control.  

---

## ✨ Features  

### **Core Medibox Functions**  
- **Time & Alarm Management**  
  - Set time zone (UTC offset).  
  - Set and manage up to 2 alarms.  
  - Snooze alarms for 5 minutes or stop them.  
  - View and delete active alarms.  
  - Fetch current time from **NTP server** over Wi-Fi and display on OLED.  

- **Environment Monitoring**  
  - Monitor **temperature** and **humidity** using **DHT11**.  
  - Warn when values exceed healthy ranges.  
  - Alerts via **buzzer**, **LED**, and OLED messages.  

---

### **Enhanced Assignment Features**  

#### 1️⃣ Light Intensity Monitoring  
- Uses **LDR** to measure light levels.  
- Samples every **5 seconds** (default).  
- Averages readings over **2 minutes** (default).  
- User can change **sampling interval (`ts`)** and **sending interval (`tu`)** from Node-RED.  
- Dashboard includes:  
  - Average light intensity (0–1 scale).  
  - Chart of historical averages.  

#### 2️⃣ Servo-Controlled Shaded Window  
- Adjusts based on **light intensity** and **temperature**.  
- Formula:  


Where:  
- θ = Motor angle  
- θoffset = Minimum angle (default 30°)  
- I = Light intensity (0–1)  
- γ = Controlling factor (default 0.75)  
- ts = Sampling interval (seconds)  
- tu = Sending interval (seconds)  
- T = Measured temperature (°C)  
- Tmed = Ideal storage temperature (default 30°C)  

#### 3️⃣ Medicine-Specific Settings  
- Node-RED sliders to adjust:  
  - **θoffset**: 0–120°  
  - **γ**: 0–1  
  - **Tmed**: 10–40°C  

---

## 🛠️ Hardware  
- ESP32 microcontroller  
- LDR (light intensity sensor)  
- DHT11 (temperature & humidity sensor)  
- Servo motor (for shade)  
- OLED display  
- Buzzer & LEDs  

---

## 💻 Software  
- Arduino / PlatformIO for ESP32 programming  
- Node-RED for dashboard UI  
- Wokwi for simulation  

---

## 📊 Node-RED Dashboard Layout  
- **Light Intensity Group**  
  - Latest average reading  
  - Historical chart  
  - Sliders for `ts` and `tu`  

- **Shaded Window Control Group**  
  - Slider for **θoffset** (min angle)  
  - Slider for **γ** (control factor)  
  - Slider for **Tmed** (ideal temperature)  

---

## 📅 Project Status  
✅ Core features complete  
✅ Light & temperature-based shade control working  
✅ Node-RED dashboard integrated  
✅ Tested in Wokwi simulation  

