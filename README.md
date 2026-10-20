# IoT Smart Multi-Room Environmental & Safety Monitoring System

[![Arduino](https://img.shields.io/badge/Firmware-Arduino%20%7C%20C%2B%2B-blue.svg)](https://www.arduino.cc/)
[![ESP8266](https://img.shields.io/badge/Hardware-ESP8266_Wi--Fi-red.svg)](https://www.espressif.com/)
[![NodeJS](https://img.shields.io/badge/Backend-Node.js%20%7C%20Express-green.svg)](https://nodejs.org/)
[![Python](https://img.shields.io/badge/Gateway-Python_3-yellow.svg)](https://www.python.org/)

An integrated IoT hardware-software safety and environmental monitoring system for multi-room facilities (e.g., student hostels, commercial buildings, dormitories). The system senses smoke, flame, anomalous acoustic events, and occupancy across multiple rooms, triggers local audio-visual alarms, broadcasts telemetry over Wi-Fi (UDP), and exposes real-time room statuses via a Node.js REST API and interactive web dashboard.

---

## Table of Contents
- [System Architecture](#system-architecture)
- [Hardware Components & Pinout](#hardware-components--pinout)
- [Software Pipeline & Communication](#software-pipeline--communication)
- [Sensor Modules & Functions](#sensor-modules--functions)
- [Web Dashboard & REST API](#web-dashboard--rest-api)
- [Setup & Installation](#setup--installation)
- [Repository Structure](#repository-structure)

---

## System Architecture

```
                                  [ Sensors ]
                   - MQ-2 Smoke/Gas (A0, A2)
                   - Sound Detection (A1)
                   - Flame IR Detectors (A3, A4)
                   - HC-SR04 Ultrasonic (Pins 4,6,9,10)
                   - Panic Reset Buttons (Pins 13, 15)
                                       │
                                       ▼
                     +-----------------------------------+
                     |      Arduino / STM32 Firmware     |
                     | - Local buzzer melody alerts      |
                     | - Room LED status indicators      |
                     | - Serial telemetry serialization  |
                     +-----------------------------------+
                                       │ (SoftwareSerial: Pins 2, 3)
                                       ▼
                     +-----------------------------------+
                     |      ESP8266 Wi-Fi Module         |
                     | - Wi-Fi Station Mode              |
                     | - UDP Datagram Broadcast          |
                     +-----------------------------------+
                                       │ (UDP Packets: Port 5000)
                                       ▼
                     +-----------------------------------+
                     |   Python Telemetry Gateway        |
                     | - Parses sensor states            |
                     | - Converts to JSON room format    |
                     +-----------------------------------+
                                       │
                                       ▼
                     +-----------------------------------+
                     |   Node.js / Express Web Server    |
                     | - GET /api/sensors                |
                     | - Serves Web Dashboard            |
                     +-----------------------------------+
                                       │
                                       ▼
                     +-----------------------------------+
                     |   Real-Time Web Dashboard         |
                     | - Live cards for Rooms 1 to 4     |
                     | - Emergency alert notifications   |
                     +-----------------------------------+
```

---

## Hardware Components & Pinout

### Arduino Pin Allocation (`combined_stuff/latest.ino`)

| Pin | Type | Connected Device | Purpose |
|:---:|:---:|:---|:---|
| `A0` | Analog In | MQ-2 Gas / Smoke Sensor (Room 1) | Flammable gas / smoke concentration |
| `A1` | Analog In | Sound / Microphone Sensor | High noise / break-in / threat detection |
| `A2` | Analog In | MQ-2 Gas / Smoke Sensor (Room 2) | Flammable gas / smoke concentration |
| `A3` | Digital In| Flame Sensor (Room 1) | Fire / IR flame detection |
| `A4` | Digital In| Flame Sensor (Room 2) | Fire / IR flame detection |
| `Pin 9` / `Pin 10` | Trig / Echo | HC-SR04 Ultrasonic (Room 1) | Room occupancy measurement (Threshold: 16 cm) |
| `Pin 4` / `Pin 6`  | Trig / Echo | HC-SR04 Ultrasonic (Room 2) | Room occupancy measurement (Threshold: 12 cm) |
| `Pin 8` | Digital Out| LED Indicator (Room 1) | Visual alert status for Room 1 |
| `Pin 11` | Digital Out| LED Indicator (Room 2) | Visual alert status for Room 2 |
| `Pin 7` | PWM Out | Piezo Buzzer | Multi-tone audible emergency siren |
| `Pin 15` / `Pin 13` | Digital In| Pushbuttons | Emergency trigger / alarm silence reset |
| `Pin 2` (RX) / `Pin 3` (TX) | Serial | ESP8266 Module | UART communication with Wi-Fi coprocessor |

---

## Software Pipeline & Communication

1. **Microcontroller Firmware:**
   Continuously reads gas levels, flame states, sound threshold, and distance pulses. When an anomaly occurs, it initiates a high-priority interruptible buzzer tone and builds a formatted status vector:
   `s1,s2,s3,s4,s5` (Smoke, Noise, Occupancy, Flame, Emergency).
2. **ESP8266 Wi-Fi Bridge (`wifi/esp8266 hardcode.ino`):**
   Receives the serial stream at 9600 baud and dispatches lightweight UDP datagrams to the gateway PC IP address on port 5000.
3. **Python Telemetry Bridge (`wifi/laptopwificode.py`):**
   Listens on UDP socket, parses discrete sensor flags, maps states to human-readable strings (`"Normal"`, `"Detected"`, `"Occupied"`, `"High"`), and exports JSON.

---

## Web Dashboard & REST API

The web application runs on Node.js using Express with CORS enabled:

### API Endpoints
- `GET /api/sensors`: Returns current sensor states for all monitored rooms in JSON format:
  ```json
  {
    "room1": "No Smoke",
    "room1Temp": "Normal",
    "room2": "Smoke Detected",
    "room2Temp": "High",
    "room3": "All Clear"
  }
  ```

### Dashboard UI
- Displays dedicated room status cards for Rooms 1 through 4.
- Dynamically updates Temperature, Smoke Level, Noise Level, and Occupancy.
- Highlights emergency warnings in an alert banner.

---

## Setup & Installation

### 1. Arduino Firmware
1. Open [`combined_stuff/latest.ino`](combined_stuff/latest.ino) in the Arduino IDE.
2. Ensure you have the `SoftwareSerial` library available.
3. Select your Arduino board and upload.

### 2. ESP8266 Wi-Fi Module
1. Open [`wifi/esp8266 hardcode.ino`](wifi/esp8266%20hardcode.ino).
2. Configure your Wi-Fi SSID, password, and the receiver PC's IP address:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   const char* udpAddress = "192.168.x.x";
   const int udpPort = 5000;
   ```
3. Upload to the ESP8266 via FTDI or NodeMCU programmer.

### 3. Node.js Web Dashboard
1. Navigate to the `website` directory:
   ```bash
   cd website
   npm install
   ```
2. Start the server:
   ```bash
   node server.js
   ```
3. Open your browser at `http://localhost:3000`.

---

## Repository Structure

```text
mpmc_project/
├── combined_stuff/            # Complete integrated firmware sketches
│   ├── latest.ino             # Main production Arduino firmware
│   ├── main.ino               # Intermediate integrated build
│   └── final.ino              # Pre-release firmware build
├── MQ2_gas_sensor/            # MQ-2 sensor test sketches & pinout schematics
├── Sound Detection/           # Sound threshold detection sketches & pinouts
├── Ultrasonic sensor/         # HC-SR04 ultrasonic distance measurement routines
├── buzzer/                    # Piezo melody code and Tinkercad simulation links
├── wifi/                      # Wireless connectivity
│   ├── esp8266 hardcode.ino   # ESP8266 UDP packet broadcaster
│   ├── laptopwificode.py      # Python UDP receiver & JSON formatter
│   └── wifistmtestcode.ino    # STM32 Wi-Fi test routines
├── website/                   # Full-stack monitoring dashboard
│   ├── server.js              # Express REST API backend
│   ├── package.json           # Node.js dependencies
│   └── frontend               # Dashboard UI logic & DOM updating
└── README.md                  # Project documentation
```
