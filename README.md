# Arduino Uno R4 WiFi Smart Clock 🕒

![Arduino Uno R4 WiFi](https://www.arduino.cc/en/uploads/Guide/UNO_R4_WIFI_hero.jpg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Arduino](https://img.shields.io/badge/Arduino-Uno_R4_WIFI-blue)

A feature-packed smart clock using **only onboard components** of the Arduino Uno R4 WiFi. Features WiFi time synchronization, motion activation, temperature monitoring, and interactive LED visualizations.

## 🌟 Features
- **Auto-synced Clock**: NTP time synchronization over WiFi
- **Motion Activation**: Wave detection via IMU accelerometer
- **Multi-mode Display**:
  - Real-time clock (HH:MM)
  - Internal temperature monitoring (°C)
  - Accelerometer visualization (moving dot)
- **Power Saving**: Auto-dims display after 1 minute of inactivity
- **No External Components**: Uses only onboard hardware

## 🛠️ Hardware Requirements
- Arduino Uno R4 WiFi
- **Onboard Components Used**:
  - 12x8 LED Matrix
  - LSM6DSOX IMU (Accelerometer)
  - User button (D12)
  - WiFi module
  - Internal temperature sensor

## 📥 Installation
1. **Clone Repository**:
   ```bash
   git clone https://github.com/your-username/Arduino-R4-WiFi-Smart-Clock.git
   cd Arduino-R4-WiFi-Smart-Clock
   Configure Secrets:
2. Create secrets.h in the project root:

#define SECRET_SSID "Your_WiFi_SSID"
#define SECRET_PASS "Your_WiFi_Password"
3. Upload Code:

Using Arduino IDE:

Install required libraries via Tools > Manage Libraries:

WiFiS3

Arduino_LSM6DSOX

RTC

LEDMatrix

Open SmartClock.ino and upload

Using PlatformIO (recommended):

pio run -t upload
🕹️ Usage
1. First Boot:

    Automatically connects to WiFi and syncs time

    Default mode: Clock display (HH:MM)

2. Controls:

    Button (D12): Cycle through modes

    Motion: Wave hand near board to wake display

    Auto-dim: Display turns off after 1 minute inactivity

3. Display Modes:

    Mode	Description	Example
    Time	HH:MM format	14:30
    Temperature	Internal chip temperature	27.5°C
    Accelerometer	Moving dot visualization	•
📂 Code Structure
    Arduino-R4-WiFi-Smart-Clock/
    ├── SmartClock/
    │   ├── SmartClock.ino       # Main application code
    │   └── secrets.h            # Wi-Fi credentials (gitignored)
    ├── platformio.ini           # PlatformIO configuration
    └── README.md                # This documentation
🔧 Configuration
    Edit these values in SmartClock.ino:


    // Timezone settings (seconds)
    const long gmtOffset = -5 * 3600;  // EST: -5h
    const int daylightOffset = 3600;

    // Motion sensitivity (1.0-2.0)
    if (abs(accelX) > 1.2 || abs(accelY) > 1.2)
🤝 Contributing
    Fork the project

    Create your feature branch:

    bash
    git checkout -b feature/awesome-feature
    Commit changes:

    bash
    git commit -m 'Add awesome feature'
    Push to branch:

    bash
    git push origin feature/awesome-feature
    Open a Pull Request

📜 License
MIT License - See LICENSE for details

🌱 Future Enhancements
Add battery power support

Implement scrolling text animations

Integrate weather API (requires external sensors)

Add alarm functionality

Made with ❤️ using Arduino Uno R4 WiFi
Let your board shine brighter than those LEDs! ✨