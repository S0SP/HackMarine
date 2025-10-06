# 🌊 Hack Marine - IoT Sensor Monitoring System

![ESP32](https://img.shields.io/badge/ESP32-Microcontroller-blue)
![Blynk](https://img.shields.io/badge/Blynk-IoT%20Platform-green)
![Arduino](https://img.shields.io/badge/Arduino-IDE-teal)

An IoT-based environmental monitoring system developed for the **Hack Marine** project presented at **Jadavpur University**. This system monitors air quality and light levels in real-time and streams data to a live web dashboard using Blynk.

---

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Circuit Diagram](#circuit-diagram)
- [Installation](#installation)
- [Configuration](#configuration)
- [Blynk Dashboard Setup](#blynk-dashboard-setup)
- [Usage](#usage)
- [Virtual Pin Mapping](#virtual-pin-mapping)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

---

## 🎯 Overview

This project is the **IoT component** of the Hack Marine initiative, designed to monitor environmental parameters critical for marine applications. The system uses an **ESP32 microcontroller** with two key sensors:

- **MQ135 Gas Sensor** - Monitors air quality by detecting harmful gases (CO2, NH3, NOx, alcohol, benzene, smoke)
- **LDR (Light Dependent Resistor)** - Detects ambient light levels and darkness

The sensor data is transmitted in real-time to a **Blynk web dashboard** for remote monitoring and analysis.

---

## ✨ Features

- 🔄 **Real-time Data Streaming** - Live sensor data updates to Blynk dashboard
- 📡 **WiFi Connectivity** - Wireless data transmission with auto-reconnection
- 🚨 **Smart Alerts** - Threshold-based detection for gas and light levels
- 💾 **Offline Mode** - Continues logging data locally when connection is lost
- 🔌 **Auto-Reconnection** - Automatically attempts to reconnect WiFi and Blynk
- 📊 **Serial Monitor Output** - Detailed sensor readings and system status
- ⚡ **Low Latency** - 3-second data refresh interval

---

## 🛠️ Hardware Requirements

| Component | Specification | Quantity |
|-----------|---------------|----------|
| ESP32 Development Board | Any ESP32 variant | 1 |
| MQ135 Gas Sensor | Air quality sensor module | 1 |
| LDR Module | Light Dependent Resistor | 1 |
| Jumper Wires | Male-to-Female | Several |
| Breadboard | Optional for prototyping | 1 |
| USB Cable | For programming ESP32 | 1 |
| Power Supply | 5V for ESP32 | 1 |

---

## 💻 Software Requirements

- **Arduino IDE** (v1.8.x or higher) or **PlatformIO**
- **ESP32 Board Package** for Arduino IDE
- **Required Libraries:**
  - `WiFi.h` (included with ESP32 board package)
  - `WiFiClient.h` (included with ESP32 board package)
  - `BlynkSimpleEsp32.h` ([Download from Blynk Library](https://github.com/blynkkk/blynk-library))

### Installing ESP32 Board in Arduino IDE:
1. Open Arduino IDE
2. Go to `File` → `Preferences`
3. Add this URL to **Additional Board Manager URLs**:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to `Tools` → `Board` → `Board Manager`
5. Search for "ESP32" and install **ESP32 by Espressif Systems**

### Installing Blynk Library:
1. Open Arduino IDE
2. Go to `Sketch` → `Include Library` → `Manage Libraries`
3. Search for "Blynk" and install **Blynk by Volodymyr Shymanskyy**

---

## 🔌 Circuit Diagram

### Pin Connections:

**MQ135 Gas Sensor:**
- VCC → 5V (or 3.3V depending on module)
- GND → GND
- AO (Analog Output) → GPIO34 (ESP32)

**LDR Module:**
- VCC → 3.3V
- GND → GND
- OUT → GPIO35 (ESP32)

```
ESP32                    Sensors
------                   -------
GPIO34 ←--[ADC]--→ MQ135 (Analog Out)
GPIO35 ←--[ADC]--→ LDR (Analog Out)
```

> **Note:** ESP32 ADC pins (GPIO34, GPIO35) are used as they support analog input. Ensure your sensors output analog signals compatible with 0-3.3V range.

---

## 📥 Installation

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/yourusername/hack-marine-iot.git
   cd hack-marine-iot
   ```

2. **Open the Code:**
   - Open `marineIOT.ino` in Arduino IDE

3. **Install Dependencies:**
   - Make sure all required libraries are installed (see Software Requirements)

4. **Configure Settings:**
   - Edit the configuration parameters (see Configuration section)

5. **Upload to ESP32:**
   - Connect ESP32 via USB
   - Select correct board: `Tools` → `Board` → `ESP32 Dev Module`
   - Select correct port: `Tools` → `Port` → `COMx` (Windows) or `/dev/ttyUSBx` (Linux/Mac)
   - Click **Upload** button

---

## ⚙️ Configuration

Before uploading, modify these settings in `marineIOT.ino`:

### 1. WiFi Credentials:
```cpp
char ssid[] = "your_wifi_name";      // Replace with your WiFi SSID
char pass[] = "your_wifi_password";  // Replace with your WiFi password
```

### 2. Blynk Configuration:
Create a new Blynk project and get your credentials from the Blynk Console:

```cpp
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"
```

> **⚠️ Important:** Never commit your actual WiFi credentials and Blynk tokens to public repositories. Use environment variables or a separate config file.

### 3. Sensor Calibration:
Adjust thresholds based on your environment:

```cpp
#define GAS_THRESHOLD 1000    // Increase if false alarms occur
#define LDR_THRESHOLD 2000    // Adjust based on ambient light
```

---

## 📱 Blynk Dashboard Setup

1. **Create Blynk Account:**
   - Go to [Blynk.Cloud](https://blynk.cloud/)
   - Sign up for a free account

2. **Create New Template:**
   - Go to Templates → `New Template`
   - Set Hardware: ESP32
   - Set Connection Type: WiFi

3. **Add Datastreams:**
   Create the following Virtual Pins:

   | Virtual Pin | Name | Data Type | Min | Max | Units |
   |-------------|------|-----------|-----|-----|-------|
   | V1 | Gas Sensor Value | Integer | 0 | 4095 | - |
   | V2 | Light Sensor Value | Integer | 0 | 4095 | - |
   | V3 | Gas Alert | Integer | 0 | 1 | - |
   | V4 | Light Block Alert | Integer | 0 | 1 | - |

4. **Create Web Dashboard:**
   - Add **Gauge** widgets for V1 (Gas Sensor) and V2 (Light Sensor)
   - Add **LED** widgets for V3 (Gas Alert) and V4 (Light Block Alert)
   - Add **Chart** widgets for historical data visualization

5. **Get Auth Token:**
   - Go to Devices → Create New Device
   - Copy the **Auth Token** and paste it in your code

---

## 🚀 Usage

1. **Power On:**
   - Connect ESP32 to power source
   - System will automatically boot and connect to WiFi

2. **Monitor Serial Output:**
   - Open Serial Monitor in Arduino IDE (`Tools` → `Serial Monitor`)
   - Set baud rate to **115200**
   - View detailed sensor readings and connection status

3. **Access Blynk Dashboard:**
   - Open Blynk app or web dashboard
   - View real-time sensor data and alerts

### Expected Serial Output:
```
==========================================
    ESP32 Gas & Light Sensor Monitor
         with Blynk Integration
==========================================

🔌 Connecting to WiFi: your_network
✓ WiFi Connected!
IP Address: 192.168.1.100
🔗 Connecting to Blynk...
✓ Blynk Connected!

📊 Sensor Configuration:
Gas Sensor: GPIO34 (MQ135)
Light Sensor: GPIO35 (LDR)
Gas Alert Threshold: 1000
Light Block Threshold: 2000

🚀 System Ready! Starting sensor monitoring...
```

---

## 📡 Virtual Pin Mapping

| Virtual Pin | Description | Value Range | Update Interval |
|-------------|-------------|-------------|-----------------|
| **V1** | Raw gas sensor reading (MQ135) | 0-4095 (12-bit ADC) | Every 3 seconds |
| **V2** | Raw light sensor reading (LDR) | 0-4095 (12-bit ADC) | Every 3 seconds |
| **V3** | Gas alert flag | 0 (Normal) / 1 (Alert) | Every 3 seconds |
| **V4** | Light block alert flag | 0 (Bright) / 1 (Dark) | Every 3 seconds |

---

## 🔧 Troubleshooting

### WiFi Connection Issues:
- ✅ Verify SSID and password are correct
- ✅ Check if ESP32 is within WiFi range
- ✅ Ensure WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- ✅ Try manual reconnection by resetting the ESP32

### Blynk Connection Issues:
- ✅ Verify Auth Token is correct
- ✅ Check if Blynk servers are online
- ✅ Ensure Virtual Pins are properly configured in Blynk template
- ✅ System will automatically attempt reconnection every 30 seconds

### Sensor Reading Issues:
- ✅ Check sensor wiring and connections
- ✅ Verify sensor power supply (3.3V or 5V depending on module)
- ✅ Calibrate threshold values based on your environment
- ✅ MQ135 requires 24-48 hours of burn-in time for accurate readings

### ESP32 Not Detected:
- ✅ Install CP210x or CH340 USB drivers
- ✅ Try different USB cable (some are power-only)
- ✅ Press and hold BOOT button during upload if needed

---

## 📊 Project Structure

```
hack-marine-iot/
│
├── marineIOT.ino          # Main Arduino code
├── README.md              # This file
└── (optional) circuit_diagram.png
```

---

## 🤝 Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📝 Future Improvements

- [ ] Add more sensor types (temperature, humidity, water quality)
- [ ] Implement data logging to SD card
- [ ] Add mobile notifications for critical alerts
- [ ] Create custom PCB for permanent installation
- [ ] Add OTA (Over-The-Air) firmware updates
- [ ] Implement MQTT protocol for alternative cloud platforms

---

## 👥 Team

This project was developed as part of the **Hack Marine** hackathon at **Jadavpur University**.

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- Blynk IoT platform for seamless cloud integration
- ESP32 community for excellent documentation
- Jadavpur University for hosting Hack Marine

---

## 📧 Contact

For questions or feedback, please open an issue on GitHub or contact the team.

---

**⭐ If you found this project helpful, please give it a star!**
