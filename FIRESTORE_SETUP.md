# Firebase Firestore Setup Guide

## Prerequisites
- Google Account
- Firebase Console Access (https://console.firebase.google.com)

## Step 1: Create Firebase Project

1. Go to [Firebase Console](https://console.firebase.google.com)
2. Click "Add Project" or "Create a Project"
3. Enter project name (e.g., "smart-energy-system")
4. Enable Google Analytics (optional)
5. Click "Create Project"

## Step 2: Setup Firestore Database

1. In the Firebase Console, navigate to "Firestore Database"
2. Click "Create Database"
3. Choose "Start in test mode" (for development)
4. Select your preferred region
5. Click "Enable"

## Step 3: Setup Android App

1. In Firebase Console, click on "Project Settings" (gear icon)
2. Click "Add App" and select Android
3. Enter package name: `com.badir.greenenergyprojectv2`
4. Register the app
5. Download `google-services.json` file
6. Place the file in: `greenenergyprojectv2/app/google-services.json`

## Step 4: Setup ESP32 (Legacy Token)

For ESP32, we need to use Firebase Realtime Database legacy tokens:

1. Go to Project Settings → Service Accounts
2. Click on "Database Secrets"
3. Copy the secret token
4. Update in `greenenergyesp32.ino`:
   ```cpp
   #define FIREBASE_HOST "YOUR_PROJECT_ID.firebaseio.com"
   #define FIREBASE_AUTH "YOUR_DATABASE_SECRET"
   ```

**Note:** Firebase has deprecated database secrets. For production, consider using:
- Firebase Cloud Functions as middleware
- MQTT Bridge
- Custom authentication server

## Step 5: Initialize Firestore Database Structure

In Firestore, create the following document structure:

### Collection: `energy_system`
### Document: `main`

```json
{
  "solar_battery": {
    "voltage": 0.0,
    "percentage": 0,
    "charging": false,
    "power_generated_today": 0.0
  },
  "piezo_battery": {
    "voltage": 0.0,
    "percentage": 0,
    "charging": false,
    "power_generated_today": 0.0
  },
  "weather_control": {
    "servo_angle": 90,
    "protection_mode": false,
    "last_weather_check": ""
  },
  "sensors": {
    "temperature": 0.0,
    "humidity": 0.0,
    "timestamp": ""
  },
  "daily_stats": {
    "total_solar_energy": 0.0,
    "total_piezo_energy": 0.0,
    "efficiency_ratio": 0.0
  }
}
```

## Step 6: Firestore Security Rules

Update your Firestore security rules:

```javascript
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {
    // Energy system collection
    match /energy_system/{document=**} {
      // Allow read for authenticated users and ESP32
      allow read: if true;
      
      // Allow write for authenticated users and ESP32
      // In production, implement proper authentication
      allow write: if true;
    }
  }
}
```

**Important:** These rules are for development only. For production:
- Implement proper authentication
- Restrict write access to authenticated ESP32 device
- Add data validation rules
- Implement rate limiting

## Step 7: Configure WiFi Credentials

Update in `greenenergyesp32.ino`:

```cpp
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
```

## Step 8: Test Connection

1. Upload the ESP32 firmware
2. Open Serial Monitor (115200 baud)
3. Verify WiFi connection
4. Check Firestore for data updates
5. Open Android app and verify real-time data display

## Alternative: Using Firebase Realtime Database

If you prefer Realtime Database over Firestore:

1. Enable Realtime Database in Firebase Console
2. Use the same database structure
3. The ESP32 code already supports Realtime Database
4. Update Android app to use Realtime Database SDK

## Troubleshooting

### ESP32 Connection Issues
- Check WiFi credentials
- Verify Firebase host URL
- Ensure database secret is correct
- Check internet connectivity

### Android App Issues
- Verify `google-services.json` is in correct location
- Check package name matches Firebase configuration
- Ensure Firestore rules allow read/write access
- Check internet permissions in AndroidManifest.xml

### Data Not Updating
- Check Firestore security rules
- Verify ESP32 serial output for errors
- Check Android app logs for Firebase errors
- Ensure device has internet connectivity

## Production Deployment Checklist

- [ ] Update Firestore security rules
- [ ] Implement proper authentication
- [ ] Add SSL certificates
- [ ] Set up monitoring and alerts
- [ ] Implement data backup strategy
- [ ] Add rate limiting
- [ ] Secure API keys
- [ ] Test error handling
- [ ] Document recovery procedures
- [ ] Set up logging and analytics
