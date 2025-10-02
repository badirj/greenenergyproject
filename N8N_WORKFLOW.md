# N8N Weather Integration Workflow

## Overview

This document describes how to set up an N8N workflow that monitors weather conditions and automatically controls the servo motor to protect solar panels during bad weather.

## What is N8N?

N8N is a workflow automation tool that allows you to connect various services and APIs together. It's similar to Zapier or IFTTT but is free and open-source.

## Installation Options

### Option 1: Cloud-hosted N8N
- Visit [n8n.cloud](https://n8n.cloud)
- Sign up for a free account
- Use their hosted service (easiest option)

### Option 2: Self-hosted with Docker
```bash
docker run -it --rm \
  --name n8n \
  -p 5678:5678 \
  -v ~/.n8n:/home/node/.n8n \
  n8nio/n8n
```

### Option 3: Self-hosted with npm
```bash
npm install n8n -g
n8n start
```

Access N8N at: http://localhost:5678

## Workflow Architecture

```
┌─────────────┐     ┌──────────────┐     ┌─────────────┐     ┌──────────────┐
│   Schedule  │────>│ Weather API  │────>│  Decision   │────>│  Firebase    │
│  (Every 30  │     │   Request    │     │   Logic     │     │   Update     │
│   minutes)  │     └──────────────┘     └─────────────┘     └──────────────┘
└─────────────┘            │                     │
                           │                     │
                    ┌──────▼─────┐        ┌─────▼──────┐
                    │  Weather   │        │   Servo    │
                    │    Data    │        │  Position  │
                    └────────────┘        └────────────┘
```

## Step-by-Step Workflow Setup

### Step 1: Create Weather API Account

**Option A: OpenWeatherMap (Recommended)**
1. Go to [OpenWeatherMap.org](https://openweathermap.org/api)
2. Sign up for a free account
3. Navigate to API keys section
4. Copy your API key
5. Free tier includes: 1000 calls/day, 60 calls/minute

**Option B: WeatherAPI.com**
1. Go to [WeatherAPI.com](https://www.weatherapi.com/)
2. Sign up for free account
3. Get API key
4. Free tier: 1 million calls/month

### Step 2: Create N8N Workflow

#### Node 1: Schedule Trigger
```
Node Type: Schedule Trigger
Settings:
  - Trigger Interval: Every 30 minutes
  - Start Time: 00:00
  - Time Zone: Your timezone
```

#### Node 2: HTTP Request - Get Weather
```
Node Type: HTTP Request
Settings:
  - Method: GET
  - URL: https://api.openweathermap.org/data/2.5/weather
  - Query Parameters:
    * lat: YOUR_LATITUDE (e.g., 41.0082)
    * lon: YOUR_LONGITUDE (e.g., 28.9784)
    * appid: YOUR_API_KEY
    * units: metric
  - Response Format: JSON
```

**Example URL:**
```
https://api.openweathermap.org/data/2.5/weather?lat=41.0082&lon=28.9784&appid=YOUR_API_KEY&units=metric
```

#### Node 3: Function - Process Weather Data
```javascript
Node Type: Function
Code:

// Extract weather data
const weather = $input.first().json;

// Extract relevant data
const temperature = weather.main.temp;
const humidity = weather.main.humidity;
const windSpeed = weather.wind.speed;
const weatherCondition = weather.weather[0].main; // Clear, Clouds, Rain, etc.
const description = weather.weather[0].description;

// Decision logic for solar panel protection
let servoAngle = 90; // Default position (facing sun)
let protectionMode = false;

// Check for bad weather conditions
if (weatherCondition === 'Rain' || 
    weatherCondition === 'Thunderstorm' || 
    weatherCondition === 'Snow' ||
    weatherCondition === 'Drizzle') {
  // Protect panels - rotate to 0 degrees (covered position)
  servoAngle = 0;
  protectionMode = true;
} else if (windSpeed > 10) {
  // High wind - protect panels
  servoAngle = 0;
  protectionMode = true;
} else if (weatherCondition === 'Clouds') {
  // Cloudy - angle at 45 degrees (partial)
  servoAngle = 45;
  protectionMode = false;
} else if (weatherCondition === 'Clear') {
  // Clear weather - full exposure
  servoAngle = 90;
  protectionMode = false;
}

// Output data
return {
  json: {
    weather: {
      temperature: temperature,
      humidity: humidity,
      windSpeed: windSpeed,
      condition: weatherCondition,
      description: description
    },
    control: {
      servoAngle: servoAngle,
      protectionMode: protectionMode,
      timestamp: new Date().toISOString()
    }
  }
};
```

#### Node 4: Firebase - Update Weather Control
```
Node Type: HTTP Request
Method: PATCH
URL: https://firestore.googleapis.com/v1/projects/YOUR_PROJECT_ID/databases/(default)/documents/energy_system/main

Authentication:
  - Type: OAuth2
  - Or use Service Account credentials

Headers:
  - Content-Type: application/json

Body (JSON):
{
  "fields": {
    "weather_control": {
      "mapValue": {
        "fields": {
          "servo_angle": {
            "integerValue": "{{$json["control"]["servoAngle"]}}"
          },
          "protection_mode": {
            "booleanValue": {{$json["control"]["protectionMode"]}}
          },
          "last_weather_check": {
            "stringValue": "{{$json["control"]["timestamp"]}}"
          }
        }
      }
    }
  }
}
```

**Alternative: Use Firebase REST API**
```
URL: https://YOUR_PROJECT_ID.firebaseio.com/energy_system.json?auth=YOUR_DATABASE_SECRET

Method: PATCH

Body:
{
  "weather_control": {
    "servo_angle": {{$json["control"]["servoAngle"]}},
    "protection_mode": {{$json["control"]["protectionMode"]}},
    "last_weather_check": "{{$json["control"]["timestamp"]}}"
  }
}
```

#### Node 5 (Optional): Send Notification
```
Node Type: HTTP Request (to notification service)
OR
Node Type: Send Email

Condition: Only if protectionMode is true

Subject: "Solar Panel Protection Activated"
Body: "Weather condition: {{$json["weather"]["condition"]}}. 
       Solar panels have been moved to protected position."
```

## Complete Workflow JSON

You can import this into N8N:

```json
{
  "nodes": [
    {
      "parameters": {
        "rule": {
          "interval": [
            {
              "triggerAtMinute": 30
            }
          ]
        }
      },
      "name": "Schedule Every 30 Minutes",
      "type": "n8n-nodes-base.scheduleTrigger",
      "position": [250, 300],
      "typeVersion": 1
    },
    {
      "parameters": {
        "url": "https://api.openweathermap.org/data/2.5/weather",
        "queryParameters": {
          "parameters": [
            {
              "name": "lat",
              "value": "YOUR_LATITUDE"
            },
            {
              "name": "lon",
              "value": "YOUR_LONGITUDE"
            },
            {
              "name": "appid",
              "value": "YOUR_API_KEY"
            },
            {
              "name": "units",
              "value": "metric"
            }
          ]
        },
        "options": {}
      },
      "name": "Get Weather",
      "type": "n8n-nodes-base.httpRequest",
      "position": [450, 300],
      "typeVersion": 1
    },
    {
      "parameters": {
        "functionCode": "// See function code above"
      },
      "name": "Process Weather Data",
      "type": "n8n-nodes-base.function",
      "position": [650, 300],
      "typeVersion": 1
    },
    {
      "parameters": {
        "url": "https://YOUR_PROJECT_ID.firebaseio.com/energy_system.json",
        "queryParameters": {
          "parameters": [
            {
              "name": "auth",
              "value": "YOUR_DATABASE_SECRET"
            }
          ]
        },
        "method": "PATCH",
        "jsonParameters": true,
        "bodyParametersJson": "={{JSON.stringify($json[\"control\"])}}"
      },
      "name": "Update Firebase",
      "type": "n8n-nodes-base.httpRequest",
      "position": [850, 300],
      "typeVersion": 1
    }
  ],
  "connections": {
    "Schedule Every 30 Minutes": {
      "main": [
        [
          {
            "node": "Get Weather",
            "type": "main",
            "index": 0
          }
        ]
      ]
    },
    "Get Weather": {
      "main": [
        [
          {
            "node": "Process Weather Data",
            "type": "main",
            "index": 0
          }
        ]
      ]
    },
    "Process Weather Data": {
      "main": [
        [
          {
            "node": "Update Firebase",
            "type": "main",
            "index": 0
          }
        ]
      ]
    }
  }
}
```

## Advanced Features

### Feature 1: Time-based Solar Tracking
```javascript
// Add to function node
const hour = new Date().getHours();

// Calculate optimal angle based on time of day
if (hour >= 6 && hour < 9) {
  servoAngle = 45; // Morning - east facing
} else if (hour >= 9 && hour < 15) {
  servoAngle = 90; // Midday - overhead
} else if (hour >= 15 && hour < 18) {
  servoAngle = 135; // Afternoon - west facing
} else {
  servoAngle = 0; // Night - protected
}

// Override with weather conditions if needed
if (protectionMode) {
  servoAngle = 0;
}
```

### Feature 2: Forecast-based Protection
```javascript
// Use forecast API endpoint
// URL: https://api.openweathermap.org/data/2.5/forecast

// Check next 3 hours forecast
const forecast = weather.list[0];
const upcomingCondition = forecast.weather[0].main;

if (upcomingCondition === 'Rain') {
  // Pre-emptively protect panels
  protectionMode = true;
  servoAngle = 0;
}
```

### Feature 3: Push Notifications
```javascript
// Add notification node
Node Type: HTTP Request
Method: POST
URL: https://onesignal.com/api/v1/notifications
// Or use FCM (Firebase Cloud Messaging)

Body:
{
  "app_id": "YOUR_APP_ID",
  "contents": {"en": "Solar panels protected due to {{weatherCondition}}"},
  "included_segments": ["All"]
}
```

## Testing the Workflow

### Manual Test
1. In N8N, click "Execute Workflow" button
2. Check execution log for errors
3. Verify Firebase data is updated
4. Check ESP32 serial output for servo movement

### Automated Test
1. Set schedule to every 1 minute for testing
2. Monitor for 5 minutes
3. Check consistency of updates
4. Reset to 30-minute interval after testing

## Troubleshooting

### Common Issues

**Issue 1: Weather API not responding**
- Check API key is valid
- Verify API quota not exceeded
- Check latitude/longitude format

**Issue 2: Firebase update failing**
- Verify database secret/credentials
- Check Firebase security rules allow writes
- Ensure URL format is correct

**Issue 3: Workflow not triggering**
- Check N8N is running
- Verify schedule trigger is active
- Check system timezone settings

**Issue 4: Servo not moving**
- Verify ESP32 is reading from Firebase
- Check serial monitor for errors
- Test servo manually from Android app

## Monitoring and Logs

### N8N Execution Logs
- View in N8N interface under "Executions"
- Enable "Save execution progress"
- Set up error notifications

### Weather Data History
Store historical weather data in Firebase:
```javascript
// Add to function node
const historyRef = `weather_history/${Date.now()}`;
// Store in separate collection for analysis
```

## Security Considerations

1. **API Keys:** Never commit API keys to version control
2. **Firebase Auth:** Use secure authentication tokens
3. **Rate Limiting:** Monitor API usage to avoid quotas
4. **HTTPS:** Always use HTTPS for API calls
5. **Validation:** Validate all data before updating Firebase

## Alternative Automation Platforms

If you prefer not to use N8N:

1. **Node-RED** - Similar to N8N, visual programming
2. **IFTTT** - Simple but limited free tier
3. **Zapier** - User-friendly but paid plans
4. **Home Assistant** - For smart home integration
5. **Custom Python Script** - For complete control

## Example Python Script Alternative

```python
import requests
import time
from firebase import firebase

# Configuration
WEATHER_API_KEY = "your_api_key"
FIREBASE_URL = "your_firebase_url"
LAT = "41.0082"
LON = "28.9784"

def get_weather():
    url = f"https://api.openweathermap.org/data/2.5/weather?lat={LAT}&lon={LON}&appid={WEATHER_API_KEY}&units=metric"
    response = requests.get(url)
    return response.json()

def update_servo(angle, protection_mode):
    fb = firebase.FirebaseApplication(FIREBASE_URL, None)
    data = {
        'servo_angle': angle,
        'protection_mode': protection_mode,
        'last_weather_check': time.strftime('%Y-%m-%d %H:%M:%S')
    }
    fb.patch('/energy_system/weather_control', data)

def main():
    while True:
        weather = get_weather()
        condition = weather['weather'][0]['main']
        
        if condition in ['Rain', 'Thunderstorm', 'Snow']:
            update_servo(0, True)
        else:
            update_servo(90, False)
        
        time.sleep(1800)  # 30 minutes

if __name__ == "__main__":
    main()
```

## Future Enhancements

1. Machine learning for optimal panel positioning
2. Integration with local weather stations
3. Historical performance analysis
4. Automated cleaning detection (dust on panels)
5. Integration with home automation systems
6. SMS alerts for critical weather events
7. Dashboard for weather trends vs energy production

## Resources

- [N8N Documentation](https://docs.n8n.io/)
- [OpenWeatherMap API Docs](https://openweathermap.org/api)
- [Firebase REST API](https://firebase.google.com/docs/reference/rest/database)
- [Servo Motor Control](https://www.arduino.cc/reference/en/libraries/servo/)
