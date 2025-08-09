# Medibox – Smart Medicine Storage & Reminder System  

## 📖 Overview  
The **Medibox** is a smart device designed to help users manage their medication schedules and ensure optimal storage conditions for sensitive medicines.  
This was developed as part of a **university semester project** and combines **time & alarm management**, **environmental monitoring**, and **automatic shading control** based on light and temperature conditions.  
The project features a **Node-RED dashboard** for configuration and monitoring, with real-time updates from an **ESP32 microcontroller**.  

---

## ✨ Features  

- **Time & Alarm Management**  
  - Set time zone (UTC offset).  
  - Set and manage up to 2 alarms.  
  - Snooze alarms for 5 minutes or stop them.  
  - View and delete active alarms.  
  - Fetch current time from **NTP server** over Wi-Fi and display on OLED.  

- **Environmental Monitoring**  
  - Monitor **temperature** and **humidity** using **DHT11**.  
  - Warn when values exceed healthy ranges.  
  - Alerts via **buzzer**, **LED**, and OLED messages.  

- **Light Intensity Monitoring**  
  - Uses **LDR** to measure light levels.  
  - Samples every **5 seconds** (default) and averages over **2 minutes** (default).  
  - User can configure **sampling interval** and **sending interval** from Node-RED.  
  - Displays latest average light intensity. 

- **Servo-Controlled Shaded Window**  
  - Adjusts automatically based on **light intensity** and **temperature**.  
  - Formula:  
    ```
    θ = θoffset + (180 − θoffset) × I × γ × ln(ts / tu) × (T / Tmed)
    ```
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
- Node-RED for dashboard UI and controls  
- Wokwi for simulation and testing  

---

## 📊 Node-RED Dashboard Layout  
- **Light Intensity Group**  
  - Latest average reading  
  - Historical chart  
  - Sliders for configrable settings.  

- **Shaded Window Control Group**  
  - Slider for **θoffset** (min angle)  
  - Slider for **γ** (control factor)  
  - Slider for **Tmed** (ideal temperature)  

---
## 🌐 Wokwi Simulation  
[Click here to open the Wokwi simulation](https://wokwi.com/projects/424932915929589761)

---

## 📅 Project Status  
✅ Core features complete  
✅ Light & temperature-based shade control working  
✅ Node-RED dashboard integrated  
✅ Tested in Wokwi simulation
