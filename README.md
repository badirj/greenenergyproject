# Smart Hybrid Energy Harvesting System

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![ESP32](https://img.shields.io/badge/ESP32-Compatible-green.svg)
![Android](https://img.shields.io/badge/Android-API%2025+-brightgreen.svg)

A complete IoT-based hybrid energy harvesting system that combines solar panels and piezoelectric disks to charge separate 18650 batteries, with weather-based protection, real-time monitoring, and mobile app control.

## 🌟 Features

- **Dual Energy Harvesting**
  - Solar panel charging (2x 6V panels in series = 12V)
  - Piezoelectric disk energy collection (6 disks in parallel)
  - Independent battery charging systems

- **Real-time Monitoring**
  - Battery voltage and percentage for both batteries
  - Temperature and humidity sensing
  - Energy production tracking
  - Live Firebase Firestore synchronization

- **Weather Protection**
  - Automatic servo motor control based on weather conditions
  - N8N workflow integration for weather API
  - Manual override via mobile app

- **Mobile Application**
  - Beautiful Material Design UI
  - Circular battery progress indicators
  - Real-time data updates
  - Manual servo control
  - Daily energy production charts

- **Local Display**
  - 16x2 LCD showing battery levels
  - Temperature and humidity display
  - System status indicators

## 📋 Table of Contents

- [Hardware Requirements](#hardware-requirements)
- [System Architecture](#system-architecture)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Documentation](#documentation)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## 🔧 Hardware Requirements

### Essential Components

| Component | Quantity | Specifications |
|-----------|----------|----------------|
| Solar Panels | 2 | 6V 100mA each |
| Piezo Disks | 6 | 27mm or 35mm diameter |
| 18650 Batteries | 2 | 3.7V 3400mAh |
| TP4056 Modules | 2 | With protection circuit |
| ESP32 DevKit V1 | 1 | WiFi + Bluetooth |
| DHT11 Sensor | 1 | Temperature/Humidity |
| SG90 Servo Motor | 1 | 180° rotation |
| 16x2 I2C LCD | 1 | With I2C backpack |
| 1N4007 Diodes | 8 | Rectifier diodes |
| Supercapacitor | 1 | 2.5V 1F |
| Resistors | 5 | 10KΩ (3x), 1KΩ (2x) |
| Push Button | 1 | Momentary SPST |

**Total Estimated Cost:** $75-80 USD

See [COMPONENTS_LIST.md](COMPONENTS_LIST.md) for detailed specifications and purchase links.

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    POWER GENERATION                          │
│                                                               │
│  Solar Panels (12V) ──→ TP4056-1 ──→ Battery 1 ──→ ESP32   │
│                                                               │
│  Piezo Disks ──→ Rectifier ──→ Supercap ──→ TP4056-2 ──→   │
│                                          Battery 2           │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                    DATA FLOW                                 │
│                                                               │
│  ESP32 ←→ Firebase Firestore ←→ Android App                │
│    ↓            ↑                                             │
│  Sensors    N8N Workflow                                     │
│  (DHT11)    (Weather API)                                    │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                    CONTROL SYSTEM                            │
│                                                               │
│  Weather Data ──→ N8N ──→ Firebase ──→ ESP32 ──→ Servo     │
│                                                    Motor     │
│                                                    (Panel    │
│                                                   Protection)│
└─────────────────────────────────────────────────────────────┘
```

## 📦 Installation

### 1. Hardware Assembly

Follow the detailed wiring instructions in [WIRING_DIAGRAM.md](WIRING_DIAGRAM.md).

**Important:** Test each component individually before final assembly!

### 2. ESP32 Firmware Setup

#### Prerequisites
- [Arduino IDE](https://www.arduino.cc/en/software) 1.8.x or later
- ESP32 Board Support Package

#### Install Required Libraries
```
1. ESP32Servo by Kevin Harrington
2. DHT sensor library by Adafruit
3. FirebaseESP32 by Mobizt
4. LiquidCrystal_I2C by Frank de Brabander
```

In Arduino IDE:
- Sketch → Include Library → Manage Libraries
- Search and install each library

#### Configure and Upload
1. Open `greenenergyesp32.ino` in Arduino IDE
2. Update WiFi credentials:
   ```cpp
   const char* WIFI_SSID = "Your_WiFi_SSID";
   const char* WIFI_PASSWORD = "Your_WiFi_Password";
   ```
3. Update Firebase credentials:
   ```cpp
   #define FIREBASE_HOST "your-project.firebaseio.com"
   #define FIREBASE_AUTH "your-database-secret"
   ```
4. Select board: Tools → Board → ESP32 Dev Module
5. Select port: Tools → Port → (Your ESP32 port)
6. Upload: Sketch → Upload

### 3. Firebase Setup

Follow the complete guide in [FIRESTORE_SETUP.md](FIRESTORE_SETUP.md).

Quick steps:
1. Create Firebase project
2. Enable Firestore Database
3. Download `google-services.json`
4. Update security rules
5. Initialize database structure

### 4. Android App Installation

#### Prerequisites
- Android Studio Electric Eel or later
- Android device/emulator with API 25+

#### Build and Install
1. Open `greenenergyprojectv2` folder in Android Studio
2. Place `google-services.json` in `app/` directory
3. Sync Gradle files
4. Build → Make Project
5. Run → Run 'app'

Or build APK:
```bash
./gradlew assembleDebug
# APK will be in app/build/outputs/apk/debug/
```

### 5. N8N Workflow Setup (Optional but Recommended)

See [N8N_WORKFLOW.md](N8N_WORKFLOW.md) for complete setup instructions.

Quick setup:
1. Install N8N (cloud or self-hosted)
2. Import workflow JSON
3. Configure Weather API credentials
4. Set schedule to run every 30 minutes
5. Connect to Firebase

## ⚙️ Configuration

### ESP32 Pin Configuration

```cpp
GPIO2  ← Button (power control)
GPIO18 → Servo (weather protection)
GPIO21 → DHT11 (temperature/humidity)
GPIO22 → LCD SDA
GPIO23 → LCD SCL
GPIO36 ← Battery 1 voltage (A0)
GPIO39 ← Battery 2 voltage (A3)
VIN    ← Battery 1 via ON/OFF button
3.3V   → Peripherals power
GND    → Common ground
```

### Firebase Database Structure

```json
{
  "energy_system": {
    "main": {
      "solar_battery": {
        "voltage": 3.85,
        "percentage": 78,
        "charging": true,
        "power_generated_today": 125.5
      },
      "piezo_battery": {
        "voltage": 3.62,
        "percentage": 65,
        "charging": false,
        "power_generated_today": 45.2
      },
      "weather_control": {
        "servo_angle": 90,
        "protection_mode": false
      },
      "sensors": {
        "temperature": 24.5,
        "humidity": 60
      },
      "daily_stats": {
        "total_solar_energy": 125.5,
        "total_piezo_energy": 45.2,
        "efficiency_ratio": 73.2
      }
    }
  }
}
```

## 🚀 Usage

### Starting the System

1. **Power On**
   - Ensure batteries are charged (>3.3V)
   - Press ON/OFF button
   - ESP32 boots and connects to WiFi
   - LCD displays "System Ready"

2. **Open Mobile App**
   - Launch app on Android device
   - Wait for "Connected" status
   - View real-time battery levels

3. **Monitor Operation**
   - Check LCD for local status
   - Use app for detailed monitoring
   - Watch servo respond to weather changes

### Daily Operation

- **Automatic Mode:** System operates autonomously
  - Solar panels charge Battery 1
  - Piezo disks charge Battery 2
  - Weather protection activates automatically
  - Data updates every 30 seconds

- **Manual Control:**
  - Adjust servo angle via app slider
  - Toggle protection mode
  - Monitor energy production
  - View daily statistics

### Power Management

- Press button to turn ESP32 on/off
- Batteries continue charging when ESP32 is off
- System automatically reconnects to WiFi on boot
- Low battery protection via TP4056 modules

## 📚 Documentation

Detailed documentation is available in the following files:

- **[COMPONENTS_LIST.md](COMPONENTS_LIST.md)** - Complete parts list with specifications and purchase links
- **[WIRING_DIAGRAM.md](WIRING_DIAGRAM.md)** - Detailed wiring instructions and schematics
- **[FIRESTORE_SETUP.md](FIRESTORE_SETUP.md)** - Firebase/Firestore configuration guide
- **[N8N_WORKFLOW.md](N8N_WORKFLOW.md)** - Weather automation setup instructions

## 🐛 Troubleshooting

### ESP32 Issues

**Problem:** ESP32 won't power on
- Check battery voltage (must be >3.3V)
- Verify button connections
- Test with USB power

**Problem:** WiFi won't connect
- Verify SSID and password
- Check 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Move closer to router

**Problem:** Data not updating to Firebase
- Check Firebase credentials
- Verify internet connection
- Review serial monitor for errors

### Android App Issues

**Problem:** App shows "Disconnected"
- Check internet connection
- Verify `google-services.json` is correct
- Check Firestore security rules

**Problem:** Battery indicators not updating
- Verify ESP32 is sending data
- Check Firebase console for data
- Restart app

### Hardware Issues

**Problem:** Batteries not charging
- Check TP4056 LED indicators (red=charging, blue=done)
- Verify solar panel voltage in sunlight
- Test piezo disks with multimeter

**Problem:** Servo not moving
- Check 3.3V power supply
- Verify GPIO18 connection
- Test servo with example sketch

**Problem:** LCD not displaying
- Check I2C address (usually 0x27 or 0x3F)
- Verify SDA/SCL connections
- Adjust contrast potentiometer

## 🎓 Educational Use

This project is ideal for:
- **Engineering students** learning IoT and renewable energy
- **Maker communities** exploring energy harvesting
- **Science fairs** demonstrating sustainable technology
- **Erasmus projects** showcasing international collaboration

### Learning Objectives
- Understanding solar and piezoelectric energy harvesting
- IoT system design and implementation
- Mobile app development with Firebase
- Workflow automation with N8N
- Hardware interfacing and sensor integration

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. **Report bugs** - Open an issue with detailed description
2. **Suggest features** - Share your ideas for improvements
3. **Submit pull requests** - Fix bugs or add features
4. **Improve documentation** - Help others understand the project
5. **Share your builds** - Show us your implementation!

### Development Setup

```bash
# Clone the repository
git clone https://github.com/badirj/greenenergyproject.git
cd greenenergyproject

# For ESP32 development
# Open greenenergyesp32.ino in Arduino IDE

# For Android development
cd greenenergyprojectv2
./gradlew build
```

## 📸 Gallery

*Add photos of your completed project here!*

## 📊 Performance Metrics

Expected performance under optimal conditions:

| Metric | Value |
|--------|-------|
| Solar charging rate | ~100-120mA in full sun |
| Piezo energy per step | ~0.5-2mJ |
| Daily solar energy | 50-150 Wh |
| Daily piezo energy | 10-50 Wh |
| ESP32 power consumption | ~80mA active, ~10mA in sleep |
| System runtime on full battery | ~48 hours |

## 🔐 Security Notes

⚠️ **Important Security Considerations:**

1. **Firebase Rules:** Update for production use
2. **API Keys:** Never commit to version control
3. **WiFi Credentials:** Use secure password storage
4. **Battery Safety:** Follow all lithium battery precautions
5. **Electrical Safety:** Double-check connections before powering on

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🙏 Acknowledgments

- ESP32 community for excellent documentation
- Firebase team for free tier Firestore
- N8N developers for workflow automation platform
- Arduino community for libraries and examples
- MPAndroidChart for beautiful chart library

## 📞 Support

For questions, issues, or suggestions:
- Open an issue on GitHub
- Contact: [Your contact information]
- Project Link: https://github.com/badirj/greenenergyproject

## 🌍 Project Background

This project was developed for the Erasmus project evaluation, demonstrating:
- **Renewable Energy:** Solar and piezoelectric harvesting
- **IoT Integration:** ESP32, Firebase, mobile apps
- **Automation:** Weather-based intelligent control
- **Sustainability:** Clean energy production and monitoring
- **Innovation:** Hybrid energy system with dual batteries

---

**Made with ❤️ for sustainable energy and IoT education**

*Last Updated: January 2025*
