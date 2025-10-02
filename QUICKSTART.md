# Quick Start Guide

Get your Smart Hybrid Energy Harvesting System running in under 30 minutes!

## Prerequisites Checklist

- [ ] All hardware components purchased (see [COMPONENTS_LIST.md](COMPONENTS_LIST.md))
- [ ] Arduino IDE installed
- [ ] Android Studio installed (for app development)
- [ ] Google account for Firebase
- [ ] Basic soldering skills

## Step 1: Set Up Firebase (10 minutes)

### 1.1 Create Firebase Project
1. Go to [Firebase Console](https://console.firebase.google.com)
2. Click "Create a project"
3. Name it "smart-energy-system"
4. Follow the wizard (enable Analytics optional)

### 1.2 Enable Firestore
1. In Firebase Console, go to "Firestore Database"
2. Click "Create database"
3. Start in **test mode** (we'll secure it later)
4. Choose your region (closest to you)
5. Click "Enable"

### 1.3 Add Android App
1. Project Settings (gear icon) → "Add app" → Android
2. Package name: `com.badir.greenenergyprojectv2`
3. Download `google-services.json`
4. Save it to: `greenenergyprojectv2/app/google-services.json`

### 1.4 Get Database Secret (for ESP32)
1. Project Settings → Service Accounts
2. Database secrets (legacy tokens)
3. Copy the secret

⚠️ **Note:** Keep this secret secure! Don't commit it to GitHub.

## Step 2: Test Components (15 minutes)

Before connecting everything, test each component individually:

### 2.1 Test Solar Panels
- Place in sunlight
- Measure voltage with multimeter
- Should read ~6V per panel, ~12V in series

### 2.2 Test Batteries
- Check voltage: should be 3.7V ± 0.3V
- Charge if needed (using TP4056 module)
- Never use fully discharged batteries

### 2.3 Test ESP32
- Connect to computer via USB
- Open Arduino IDE
- Upload Blink example
- Verify it works

### 2.4 Test DHT11
```cpp
#include <DHT.h>
#define DHT_PIN 21
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Serial.print("Temp: "); Serial.print(temp);
  Serial.print(" Humidity: "); Serial.println(hum);
  delay(2000);
}
```

### 2.5 Test Servo
```cpp
#include <ESP32Servo.h>
Servo servo;

void setup() {
  servo.attach(18);
  servo.write(90);
}

void loop() {
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(180);
  delay(1000);
}
```

### 2.6 Test LCD
```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin(22, 23); // SDA, SCL
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LCD Test OK!");
}

void loop() {}
```

## Step 3: Assemble Hardware (30 minutes)

### 3.1 Solar Circuit
```
Solar Panel 1 (+) ──┬── Solar Panel 2 (+) ──┬── TP4056-1 IN+
                    │                        │
Solar Panel 1 (-) ──┘                        │
                                             │
                 Solar Panel 2 (-) ──────────┴── TP4056-1 IN-

Battery 1 (+) ──→ TP4056-1 B+
Battery 1 (-) ──→ TP4056-1 B-
```

### 3.2 Voltage Dividers
```
Battery 1 (+) ──┬── 10KΩ ──┬── GPIO36
                │           │
                └── 1KΩ ────┴── GND

Battery 2 (+) ──┬── 10KΩ ──┬── GPIO39
                │           │
                └── 1KΩ ────┴── GND
```

### 3.3 ESP32 Connections (minimal for testing)
```
VIN    ← Battery 1 OUT+ (via button)
GND    ← Common Ground
GPIO2  ← Button (with pull-up)
GPIO18 → Servo Signal
GPIO21 → DHT11 Data
GPIO22 → LCD SDA
GPIO23 → LCD SCL
GPIO36 ← Battery 1 voltage divider
3.3V   → DHT11 VCC, LCD VCC, Servo VCC
```

⚠️ **Safety Check:**
- Verify all connections with multimeter
- Check for shorts
- Ensure correct polarity on batteries
- Double-check voltage divider outputs (<3.3V)

## Step 4: Program ESP32 (10 minutes)

### 4.1 Install Libraries
In Arduino IDE → Sketch → Include Library → Manage Libraries:
- `ESP32Servo` by Kevin Harrington
- `DHT sensor library` by Adafruit
- `Adafruit Unified Sensor` (dependency)
- `FirebaseESP32` by Mobizt
- `LiquidCrystal_I2C` by Frank de Brabander

### 4.2 Configure ESP32 Code
Open `greenenergyesp32.ino` and update:
```cpp
const char* WIFI_SSID = "YourWiFiName";
const char* WIFI_PASSWORD = "YourWiFiPassword";

#define FIREBASE_HOST "your-project-id.firebaseio.com"
#define FIREBASE_AUTH "your-database-secret-from-step-1.4"
```

### 4.3 Upload
1. Tools → Board → ESP32 Dev Module
2. Tools → Port → (Select your ESP32)
3. Sketch → Upload
4. Open Serial Monitor (115200 baud)
5. Watch for "System Ready"

## Step 5: Initialize Firestore (5 minutes)

In Firebase Console → Firestore Database → Start collection:

**Collection ID:** `energy_system`
**Document ID:** `main`

Add these fields (click "Add field" for each):

```javascript
solar_battery (map)
  ├─ voltage (number): 0.0
  ├─ percentage (number): 0
  ├─ charging (boolean): false
  └─ power_generated_today (number): 0.0

piezo_battery (map)
  ├─ voltage (number): 0.0
  ├─ percentage (number): 0
  ├─ charging (boolean): false
  └─ power_generated_today (number): 0.0

weather_control (map)
  ├─ servo_angle (number): 90
  ├─ protection_mode (boolean): false
  └─ last_weather_check (string): ""

sensors (map)
  ├─ temperature (number): 0.0
  ├─ humidity (number): 0.0
  └─ timestamp (string): ""

daily_stats (map)
  ├─ total_solar_energy (number): 0.0
  ├─ total_piezo_energy (number): 0.0
  └─ efficiency_ratio (number): 0.0
```

**Quick way:** Use the Firestore web interface to manually enter one complete document.

## Step 6: Build Android App (15 minutes)

### 6.1 Open Project
1. Open Android Studio
2. File → Open → Select `greenenergyprojectv2` folder
3. Wait for Gradle sync

### 6.2 Verify Configuration
1. Check `app/google-services.json` exists
2. Build → Make Project
3. Fix any errors (should compile cleanly)

### 6.3 Install on Device
1. Enable Developer Mode on Android device
2. Enable USB Debugging
3. Connect device to computer
4. Run → Run 'app'
5. Select your device
6. Wait for installation

## Step 7: First Test (5 minutes)

### 7.1 Power On System
1. Press power button
2. ESP32 should boot (watch Serial Monitor)
3. LCD should show "System Ready"
4. Wait for WiFi connection

### 7.2 Check Firebase
1. Open Firebase Console
2. Go to Firestore Database
3. Watch `energy_system/main` document
4. Values should update every 30 seconds

### 7.3 Open Android App
1. Launch app on phone
2. Should show "Connected" at top
3. Battery percentages should appear
4. Temperature and humidity should update

### 7.4 Test Servo Control
1. In app, move servo slider
2. Watch servo motor move
3. Check angle updates in Firebase
4. Verify ESP32 Serial Monitor shows angle change

## Troubleshooting Quick Fixes

### ESP32 Won't Boot
- Check battery voltage (need >3.3V)
- Try USB power instead
- Verify button connection

### WiFi Won't Connect
- Double-check SSID and password
- Use 2.4GHz network (not 5GHz)
- Move closer to router

### App Shows "Disconnected"
- Check internet connection
- Verify `google-services.json` is correct
- Check Firebase security rules allow read/write

### LCD Shows Nothing
- Check I2C address (try 0x3F if 0x27 doesn't work)
- Adjust contrast potentiometer
- Verify 3.3V power

### Servo Doesn't Move
- Check GPIO18 connection
- Verify servo has power
- Test with example sketch

### No Data in Firebase
- Check WiFi connection
- Verify Firebase credentials
- Look for errors in Serial Monitor
- Check Firebase security rules

## Next Steps

✅ **System is working!** Now you can:

1. **Add Piezo Circuit** - Follow full wiring diagram
2. **Set up N8N** - For weather automation
3. **Optimize Placement** - Position solar panels for max sun
4. **Calibrate** - Fine-tune voltage readings
5. **Monitor** - Track energy production over days
6. **Secure** - Update Firebase security rules for production

## Need More Help?

- **Detailed Wiring:** See [WIRING_DIAGRAM.md](WIRING_DIAGRAM.md)
- **Component Info:** See [COMPONENTS_LIST.md](COMPONENTS_LIST.md)
- **Firebase Setup:** See [FIRESTORE_SETUP.md](FIRESTORE_SETUP.md)
- **Weather Automation:** See [N8N_WORKFLOW.md](N8N_WORKFLOW.md)
- **Full Docs:** See [README.md](README.md)

## Success Checklist

- [ ] Firebase project created and configured
- [ ] All components tested individually
- [ ] Hardware assembled and checked for shorts
- [ ] ESP32 programmed and connected to WiFi
- [ ] Firestore database initialized with structure
- [ ] Android app built and installed
- [ ] Real-time data flowing: ESP32 → Firebase → App
- [ ] Servo control working from app
- [ ] LCD displaying battery levels
- [ ] Batteries charging from solar panels

**Congratulations! Your Smart Hybrid Energy Harvesting System is operational! 🎉**

---

**Estimated Total Time:** 90 minutes (excluding component ordering)
**Difficulty Level:** Intermediate
**Cost:** ~$75-80 USD

*For Erasmus Project Evaluation - Demonstrating IoT, Renewable Energy, and Mobile Development Skills*
