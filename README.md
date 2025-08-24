
# 🏢 SmartZone - Intelligent People-Counting & Zone Management System

![SmartZone Banner](https://img.shields.io/badge/Project-SmartZone-blue.svg)
![IoT](https://img.shields.io/badge/IoT-Enabled-green.svg)
![ESP8266](https://img.shields.io/badge/NodeMCU-ESP8266-red.svg)
![Blynk](https://img.shields.io/badge/Cloud-Blynk-yellow.svg)

An IoT-based automated system that monitors occupancy using IR sensors and controls appliances based on real-time people count. Perfect for smart classrooms, offices, and energy-efficient buildings.

![WhatsApp Image 2025-08-24 at 22 43 30_c046023e](https://github.com/user-attachments/assets/7467d408-b95f-48c3-ba2c-b52ce6da57de)
![WhatsApp Image 2025-08-24 at 22 43 30_8ea7a100](https://github.com/user-attachments/assets/9ecf8115-8811-4b38-8a98-32d979d8addb)


---

## 🌟 Features

### 🔢 People Counting
- **Dual IR Sensor System** - Accurate entry/exit detection
- **Real-time Counting** - Instant updates on LCD and Blynk app
- **Debounce Logic** - Prevents false triggers

### 🎯 Zone Automation
- **4 Independent Zones** with configurable thresholds:
  - Zone 1: ≥1 person
  - Zone 2: ≥10 people  
  - Zone 3: ≥20 people
  - Zone 4: ≥25 people
- **Relay Control** - Manages lights, fans, ACs automatically

### 🌡️ Environmental Monitoring
- **DHT11 Sensor** - Real-time temperature & humidity tracking
- **Blynk Integration** - Remote monitoring capabilities

### 📱 Remote Control
- **Blynk IoT App** - Full remote control and monitoring
- **Manual Override** - Switch between Auto/Manual modes
- **Real-time Dashboard** - Live data visualization

### 🔄 Connectivity Features
- **WiFi Resilience** - Works offline, auto-reconnects
- **Local Operation** - Functions without internet
- **Optimized Data** - Minimal Blynk data usage

---

## 🛠️ Hardware Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| NodeMCU ESP8266 | 1 | Main microcontroller |
| IR Obstacle Sensors | 2 | People detection |
| 4-Channel Relay Module | 1 | Appliance control |
| DHT11 Sensor | 1 | Temperature & humidity |
| I2C LCD (16x2) | 1 | Local display |
| Breadboard & Jumper Wires | - | Connections |
| 5V Power Supply | 1 | System power |

---

## 📋 Pin Configuration

| NodeMCU Pin | Component | Note |
|-------------|-----------|------|
| D5 | IR Sensor A (Entry) | INPUT_PULLUP |
| D6 | IR Sensor B (Exit) | INPUT_PULLUP |
| D0 | Relay Zone 1 | OUTPUT |
| D7 | Relay Zone 2 | OUTPUT |
| D3 | Relay Zone 3 | OUTPUT |
| D8 | Relay Zone 4 | OUTPUT |
| D4 | DHT11 Sensor | INPUT_PULLUP |
| D1 (SCL) | I2C LCD | I2C Clock |
| D2 (SDA) | I2C LCD | I2C Data |

---

## 🚀 Installation & Setup

### 1. Library Installation
Install required Arduino libraries:
```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
```

### 2. Hardware Setup
1. Connect components as per pin configuration
2. Use 5V power supply for relays
3. Ensure proper grounding

### 3. Software Configuration
1. Update WiFi credentials in code:
```cpp
#define WIFI_SSID "your_SSID"
#define WIFI_PASSWORD "your_PASSWORD"
```

2. Set Blynk authentication token:
```cpp
#define BLYNK_AUTH_TOKEN "your_AUTH_TOKEN"
```

### 4. Blynk Setup
1. Create new project in Blynk app
2. Add following widgets:
   - V0: People Counter (Value Display)
   - V1-V4: Zone LEDs (LED)
   - V5-V7, V9: Zone Switches (Button)
   - V8: Mode Switch (Button)
   - V10: Temperature (Gauge)
   - V11: Humidity (Gauge)

---

## 📊 Blynk Virtual Pin Mapping

| Virtual Pin | Function | Type |
|-------------|----------|------|
| V0 | People Count | Value Display |
| V1 | Zone 1 Status | LED |
| V2 | Zone 2 Status | LED |
| V3 | Zone 3 Status | LED |
| V4 | Zone 4 Status | LED |
| V5 | Zone 1 Control | Button |
| V6 | Zone 2 Control | Button |
| V7 | Zone 3 Control | Button |
| V9 | Zone 4 Control | Button |
| V8 | Auto/Manual Mode | Switch |
| V10 | Temperature | Gauge |
| V11 | Humidity | Gauge |

---

## ⚙️ How It Works

### 🔄 Operation Modes
1. **Auto Mode**: Zones activate based on people count thresholds
2. **Manual Mode**: Direct control via Blynk app

### 📶 Connectivity
- **Online**: Full Blynk integration with real-time updates
- **Offline**: Local operation with LCD display
- **Auto-reconnect**: Automatic WiFi recovery

### ⚡ Optimization Features
- **Data Throttling**: Only sends updates on significant changes
- **Debounced Inputs**: Prevents false sensor triggers
- **State Tracking**: Avoids redundant Blynk updates

---

## 🎯 Applications

- **Smart Classrooms** - Automated lighting and fans
- **Office Spaces** - Energy-efficient AC control
- **Conference Halls** - Zone-wise power management
- **Public Libraries** - Occupancy-based lighting
- **Retail Stores** - Customer counting and analytics

---

## 🔧 Troubleshooting

### Common Issues:
1. **WiFi Connection Failed**
   - Check SSID/password
   - Verify router settings

2. **Sensors Not Triggering**
   - Check IR sensor alignment
   - Verify pull-up resistors

3. **Blynk Not Connecting**
   - Verify auth token
   - Check internet connection

4. **Relays Not Switching**
   - Check power supply
   - Verify relay module type (Active LOW)

---

## 🌟 Future Enhancements

- [ ] Facial recognition for accurate counting
- [ ] Solar power integration
- [ ] Voice control (Alexa/Google Assistant)
- [ ] Data logging to cloud
- [ ] Mobile app notifications
- [ ] Energy consumption analytics

---
## 📄 License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Feel free to check [issues page](https://github.com/MaxonXOXO/our-smartzone/issues).

---

## 📞 Support

If you have any questions or need help setting up, please:
Contact us at: wizardmaxps@gmail.com

---

## 🙏 Acknowledgments

- **Blynk** for IoT platform
- **Arduino** community for libraries
- **College/University** for support and resources

---

**⭐ Star this repo if you found it helpful!**
---
***Made with 💗 by MaxonXOXO
