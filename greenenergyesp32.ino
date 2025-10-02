/*
 * Smart Hybrid Energy Harvesting System
 * ESP32 DevKit V1 Firmware
 * 
 * Hardware Configuration:
 * - 2x 6V 100mA Solar Panels (series) -> Battery 1 via TP4056
 * - 6x Piezo Disks (parallel) -> Battery 2 via TP4056
 * - DHT11 Temperature/Humidity Sensor
 * - SG90 Servo Motor (Solar Panel Protection)
 * - 16x2 I2C LCD Display
 * - ON/OFF Button for Power Control
 * 
 * Pin Connections:
 * VIN    <- ON/OFF Button <- Battery-1 (+)
 * GND    <- Common Ground
 * GPIO2  <- Button Input (pull-up)
 * GPIO18 -> Servo PWM
 * GPIO21 -> DHT11 Data
 * GPIO22 -> LCD SDA
 * GPIO23 -> LCD SCL
 * A0     <- Battery-1 Voltage (10K+1K divider)
 * A3     <- Battery-2 Voltage (10K+1K divider)
 * 3.3V   -> DHT11 VCC, LCD VCC
 */

#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

// WiFi Credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Firebase Credentials
#define FIREBASE_HOST "YOUR_PROJECT_ID.firebaseio.com"
#define FIREBASE_AUTH "YOUR_DATABASE_SECRET"

// Pin Definitions
#define BUTTON_PIN 2
#define SERVO_PIN 18
#define DHT_PIN 21
#define LCD_SDA 22
#define LCD_SCL 23
#define BATTERY1_PIN 36  // A0 - ADC1_CH0 (GPIO36)
#define BATTERY2_PIN 39  // A3 - ADC1_CH3 (GPIO39)

// Component Configuration
#define DHT_TYPE DHT11
#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// Voltage Divider Configuration (10K + 1K)
#define R1 10000.0  // 10K resistor
#define R2 1000.0   // 1K resistor
#define ADC_MAX 4095.0
#define ADC_VREF 3.3

// Battery Specifications (18650 Li-ion)
#define BATTERY_MAX_VOLTAGE 4.2
#define BATTERY_MIN_VOLTAGE 3.0

// Timing
#define UPDATE_INTERVAL 30000  // 30 seconds
#define DEBOUNCE_DELAY 50

// Objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;
Servo servoMotor;
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// Global Variables
unsigned long lastUpdate = 0;
unsigned long lastButtonPress = 0;
bool systemActive = true;
float battery1Voltage = 0.0;
float battery2Voltage = 0.0;
int battery1Percentage = 0;
int battery2Percentage = 0;
float temperature = 0.0;
float humidity = 0.0;
int currentServoAngle = 90;
bool protectionMode = false;

// Daily Energy Tracking
float dailySolarEnergy = 0.0;
float dailyPiezoEnergy = 0.0;
unsigned long lastEnergyUpdate = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Smart Hybrid Energy Harvesting System");
  Serial.println("Initializing...");
  
  // Initialize pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BATTERY1_PIN, INPUT);
  pinMode(BATTERY2_PIN, INPUT);
  
  // Initialize I2C
  Wire.begin(LCD_SDA, LCD_SCL);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Energy System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  // Initialize DHT sensor
  dht.begin();
  delay(2000);
  
  // Initialize Servo
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(90);  // Center position
  delay(500);
  
  // Connect to WiFi
  connectWiFi();
  
  // Configure Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);
  
  // Set time for timestamps
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1000);
  
  Serial.println("Initialization complete!");
}

void loop() {
  // Check button for power control
  checkButton();
  
  if (!systemActive) {
    lcd.noBacklight();
    delay(100);
    return;
  }
  
  // Update data every 30 seconds
  if (millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    
    // Read sensors
    readBatteryVoltages();
    readDHTSensor();
    
    // Update LCD
    updateLCD();
    
    // Read servo angle from Firebase (weather protection)
    readServoCommand();
    
    // Send data to Firebase
    sendDataToFirebase();
    
    // Calculate daily energy
    calculateDailyEnergy();
  }
  
  delay(100);
}

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  lcd.setCursor(0, 1);
  lcd.print("WiFi Connecting");
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    lcd.setCursor(0, 1);
    lcd.print("WiFi Connected ");
  } else {
    Serial.println("\nWiFi Connection Failed!");
    lcd.setCursor(0, 1);
    lcd.print("WiFi Failed    ");
  }
  delay(1000);
}

void checkButton() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
      systemActive = !systemActive;
      lastButtonPress = millis();
      
      if (systemActive) {
        lcd.backlight();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("System ON");
        Serial.println("System activated");
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("System OFF");
        Serial.println("System deactivated");
      }
      delay(1000);
    }
  }
}

void readBatteryVoltages() {
  // Read Battery 1 (Solar)
  int adc1 = analogRead(BATTERY1_PIN);
  battery1Voltage = (adc1 / ADC_MAX) * ADC_VREF * ((R1 + R2) / R2);
  battery1Percentage = calculateBatteryPercentage(battery1Voltage);
  
  // Read Battery 2 (Piezo)
  int adc2 = analogRead(BATTERY2_PIN);
  battery2Voltage = (adc2 / ADC_MAX) * ADC_VREF * ((R1 + R2) / R2);
  battery2Percentage = calculateBatteryPercentage(battery2Voltage);
  
  Serial.print("Battery 1 (Solar): ");
  Serial.print(battery1Voltage);
  Serial.print("V (");
  Serial.print(battery1Percentage);
  Serial.println("%)");
  
  Serial.print("Battery 2 (Piezo): ");
  Serial.print(battery2Voltage);
  Serial.print("V (");
  Serial.print(battery2Percentage);
  Serial.println("%)");
}

int calculateBatteryPercentage(float voltage) {
  if (voltage >= BATTERY_MAX_VOLTAGE) return 100;
  if (voltage <= BATTERY_MIN_VOLTAGE) return 0;
  
  float percentage = ((voltage - BATTERY_MIN_VOLTAGE) / (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0;
  return (int)constrain(percentage, 0, 100);
}

void readDHTSensor() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = 0.0;
    humidity = 0.0;
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  }
}

void updateLCD() {
  lcd.clear();
  
  // First row: Battery levels
  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(battery1Percentage);
  lcd.print("% P:");
  lcd.print(battery2Percentage);
  lcd.print("%");
  
  // Second row: Temperature and humidity
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print((int)temperature);
  lcd.print("C H:");
  lcd.print((int)humidity);
  lcd.print("%");
}

void readServoCommand() {
  if (Firebase.getInt(firebaseData, "/energy_system/weather_control/servo_angle")) {
    int angle = firebaseData.intData();
    if (angle >= 0 && angle <= 180 && angle != currentServoAngle) {
      currentServoAngle = angle;
      servoMotor.write(angle);
      Serial.print("Servo moved to: ");
      Serial.println(angle);
    }
  }
  
  if (Firebase.getBool(firebaseData, "/energy_system/weather_control/protection_mode")) {
    protectionMode = firebaseData.boolData();
  }
}

void sendDataToFirebase() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, attempting to reconnect...");
    connectWiFi();
    return;
  }
  
  // Solar Battery Data
  Firebase.setFloat(firebaseData, "/energy_system/solar_battery/voltage", battery1Voltage);
  Firebase.setInt(firebaseData, "/energy_system/solar_battery/percentage", battery1Percentage);
  Firebase.setBool(firebaseData, "/energy_system/solar_battery/charging", battery1Voltage > 3.7);
  Firebase.setFloat(firebaseData, "/energy_system/solar_battery/power_generated_today", dailySolarEnergy);
  
  // Piezo Battery Data
  Firebase.setFloat(firebaseData, "/energy_system/piezo_battery/voltage", battery2Voltage);
  Firebase.setInt(firebaseData, "/energy_system/piezo_battery/percentage", battery2Percentage);
  Firebase.setBool(firebaseData, "/energy_system/piezo_battery/charging", battery2Voltage > 3.7);
  Firebase.setFloat(firebaseData, "/energy_system/piezo_battery/power_generated_today", dailyPiezoEnergy);
  
  // Weather Control Data
  Firebase.setInt(firebaseData, "/energy_system/weather_control/servo_angle", currentServoAngle);
  Firebase.setBool(firebaseData, "/energy_system/weather_control/protection_mode", protectionMode);
  Firebase.setString(firebaseData, "/energy_system/weather_control/last_weather_check", getTimestamp());
  
  // Sensor Data
  Firebase.setFloat(firebaseData, "/energy_system/sensors/temperature", temperature);
  Firebase.setFloat(firebaseData, "/energy_system/sensors/humidity", humidity);
  Firebase.setString(firebaseData, "/energy_system/sensors/timestamp", getTimestamp());
  
  // Daily Stats
  Firebase.setFloat(firebaseData, "/energy_system/daily_stats/total_solar_energy", dailySolarEnergy);
  Firebase.setFloat(firebaseData, "/energy_system/daily_stats/total_piezo_energy", dailyPiezoEnergy);
  float efficiencyRatio = (dailySolarEnergy > 0) ? (dailySolarEnergy / (dailySolarEnergy + dailyPiezoEnergy)) * 100 : 0;
  Firebase.setFloat(firebaseData, "/energy_system/daily_stats/efficiency_ratio", efficiencyRatio);
  
  Serial.println("Data sent to Firebase successfully!");
}

void calculateDailyEnergy() {
  // Simple energy calculation based on voltage and time
  // Energy (Wh) = Voltage * Current * Time
  // This is a simplified calculation for demonstration
  unsigned long timeDelta = millis() - lastEnergyUpdate;
  if (lastEnergyUpdate > 0) {
    float hours = timeDelta / 3600000.0;
    
    // Assume average current of 50mA when charging
    if (battery1Voltage > 3.7) {
      dailySolarEnergy += battery1Voltage * 0.05 * hours;
    }
    if (battery2Voltage > 3.7) {
      dailyPiezoEnergy += battery2Voltage * 0.05 * hours;
    }
  }
  lastEnergyUpdate = millis();
  
  // Reset daily energy at midnight (simplified - resets after 24 hours)
  if (millis() > 86400000) {  // 24 hours in milliseconds
    dailySolarEnergy = 0;
    dailyPiezoEnergy = 0;
  }
}

String getTimestamp() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}
