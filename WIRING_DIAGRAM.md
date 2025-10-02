# Wiring Diagram and Pin Connections

## System Overview

This document provides detailed wiring instructions for the Smart Hybrid Energy Harvesting System.

## Component List Summary

- 2x 6V 100mA Solar Panels
- 6x Piezo Disks
- 2x TP4056 Charge Modules
- 2x 18650 Li-ion Batteries (3.7V 3400mAh)
- 1x ESP32 DevKit V1
- 1x DHT11 Sensor
- 1x SG90 Servo Motor
- 1x 16x2 I2C LCD Display
- 1x ON/OFF Button
- 8x 1N4007 Diodes
- 1x 2.5V 1F Supercapacitor
- Resistors: 2x 10KΩ, 2x 1KΩ (for voltage dividers)

## Part 1: Solar Panel Circuit

### Solar Panel Connection (Series)
```
Panel 1 (+6V) ──┬─── Panel 2 (+6V) ──┬─── To TP4056-1 IN+
                │                      │
Panel 1 (GND) ──┘                      │
                                       │
                Panel 2 (GND) ─────────┴─── To TP4056-1 IN-

Result: 12V @ 100mA
```

### TP4056 Module 1 (Solar Charging)
```
IN+ ← Solar Panels (+12V)
IN- ← Solar Panels (GND)
B+ → Battery 1 (+)
B- → Battery 1 (-)
OUT+ → ESP32 VIN (via ON/OFF button)
OUT- → Common Ground
```

## Part 2: Piezoelectric Circuit

### Piezo Disk Rectifier (Per Disk)
```
Each Piezo Disk needs a bridge rectifier:

Piezo (+) ───┬──────┬───┐
             │      │   │
             D1    │   │
             │      │   │
             └──────┼───┴─── (+) to Supercapacitor
                    │
Piezo (-) ───┬──────┤
             │      │
             D2    │
             │      │
             └──────┴─────── (-) to Supercapacitor

Use 1N4007 diodes for D1 and D2
Repeat for all 6 piezo disks in parallel
```

### All Piezo Disks in Parallel
```
Piezo1 (+) ──┐
Piezo2 (+) ──┤
Piezo3 (+) ──┼─── Rectified (+) ─── Supercapacitor (+)
Piezo4 (+) ──┤
Piezo5 (+) ──┤
Piezo6 (+) ──┘

Piezo1 (-) ──┐
Piezo2 (-) ──┤
Piezo3 (-) ──┼─── Rectified (-) ─── Supercapacitor (-)
Piezo4 (-) ──┤
Piezo5 (-) ──┤
Piezo6 (-) ──┘
```

### Supercapacitor to TP4056 Module 2
```
Supercapacitor (+) ─── 1N4007 ─── TP4056-2 IN+
Supercapacitor (-) ──────────────── TP4056-2 IN-

Note: Diode prevents backflow to supercapacitor
```

### TP4056 Module 2 (Piezo Charging)
```
IN+ ← Supercapacitor (+) via 1N4007
IN- ← Supercapacitor (-)
B+ → Battery 2 (+)
B- → Battery 2 (-)
OUT+ → Not used (keeps battery isolated)
OUT- → Common Ground
```

## Part 3: ESP32 Connections

### Power Supply
```
ESP32 VIN ← ON/OFF Button ← Battery 1 OUT+ (TP4056-1)
ESP32 GND ← Common Ground
```

### Digital Pins
```
GPIO2  ← Button (with 10KΩ pull-up to 3.3V)
GPIO18 → Servo Signal (Orange/Yellow wire)
GPIO21 → DHT11 Data Pin
GPIO22 → LCD SDA
GPIO23 → LCD SCL
```

### Analog Pins (Voltage Monitoring)
```
GPIO36 (A0) ← Battery 1 Voltage Divider
GPIO39 (A3) ← Battery 2 Voltage Divider
```

### Power Distribution
```
ESP32 3.3V → DHT11 VCC
ESP32 3.3V → LCD VCC
ESP32 3.3V → Servo VCC (Red wire)
ESP32 GND → DHT11 GND
ESP32 GND → LCD GND
ESP32 GND → Servo GND (Brown/Black wire)
```

## Part 4: Voltage Dividers

### Battery 1 Voltage Divider
```
Battery 1 (+) ───┬─── 10KΩ resistor ───┬─── GPIO36 (A0)
                 │                      │
                 │     1KΩ resistor     │
                 │                      │
                 └──────────────────────┴─── GND

Calculation: Vout = Vin × (R2 / (R1 + R2))
Max Input: 4.2V
Output to ESP32: 4.2V × (1K / 11K) = 0.38V (safe for ESP32)
```

### Battery 2 Voltage Divider
```
Battery 2 (+) ───┬─── 10KΩ resistor ───┬─── GPIO39 (A3)
                 │                      │
                 │     1KΩ resistor     │
                 │                      │
                 └──────────────────────┴─── GND
```

## Part 5: ON/OFF Button Circuit

### Power Button
```
Battery 1 OUT+ ───┬─── Button ─── ESP32 VIN
                  │
                  └─── 10KΩ ─── ESP32 GPIO2

When button is pressed: Powers ESP32
GPIO2 reads LOW when button pressed
```

**Note:** This is a simple momentary button. For latching power control, consider using a P-channel MOSFET circuit.

## Part 6: Complete System Schematic

```
┌─────────────────────────────────────────────────────────────┐
│                    SOLAR SECTION                             │
│  Solar Panel 1 ──┬── Solar Panel 2 ── TP4056-1 ── Battery 1 │
│                  (Series: 12V)              ↓                │
│                                          Button              │
│                                             ↓                │
│                                          ESP32 VIN           │
└──────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                    PIEZO SECTION                             │
│  Piezo 1 ──┐                                                 │
│  Piezo 2 ──┤                                                 │
│  Piezo 3 ──┼── Rectifiers ── Supercap ── TP4056-2 ── Bat 2  │
│  Piezo 4 ──┤    (8 diodes)                                   │
│  Piezo 5 ──┤                                                 │
│  Piezo 6 ──┘                                                 │
└──────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                    ESP32 SECTION                             │
│                                                               │
│  GPIO2  ← Button                                             │
│  GPIO18 → Servo Motor                                        │
│  GPIO21 → DHT11 Sensor                                       │
│  GPIO22 → LCD SDA                                            │
│  GPIO23 → LCD SCL                                            │
│  GPIO36 ← Battery 1 Voltage (via divider)                   │
│  GPIO39 ← Battery 2 Voltage (via divider)                   │
│  3.3V   → DHT11, LCD, Servo                                  │
│  GND    → Common Ground                                      │
└──────────────────────────────────────────────────────────────┘
```

## Assembly Instructions

### Step 1: Prepare Solar Circuit
1. Connect solar panels in series (positive to negative)
2. Connect to TP4056-1 input
3. Connect Battery 1 to TP4056-1 battery terminals
4. Test voltage with multimeter (should show ~12V)

### Step 2: Prepare Piezo Circuit
1. Create rectifier bridges for each piezo disk
2. Connect all rectified outputs in parallel
3. Connect to supercapacitor
4. Add protection diode to TP4056-2 input
5. Connect Battery 2 to TP4056-2 battery terminals

### Step 3: Wire ESP32
1. Create voltage dividers on breadboard/PCB
2. Connect all digital pins as per diagram
3. Wire power button between Battery 1 OUT+ and ESP32 VIN
4. Connect button sense to GPIO2

### Step 4: Connect Peripherals
1. Wire DHT11 sensor (VCC, GND, Data)
2. Wire LCD with I2C (VCC, GND, SDA, SCL)
3. Wire servo motor (VCC, GND, Signal)
4. Double-check all connections

### Step 5: Test Power
1. Charge batteries separately first
2. Test voltage dividers with multimeter
3. Power on ESP32 and verify operation
4. Check serial output for errors

## Safety Notes

⚠️ **IMPORTANT SAFETY PRECAUTIONS:**

1. **Battery Safety:**
   - Never short circuit 18650 batteries
   - Use batteries with protection circuits
   - Monitor temperature during charging
   - Keep away from metal objects

2. **Voltage Safety:**
   - Verify voltage dividers before connecting to ESP32
   - ESP32 ADC maximum input is 3.3V
   - Never exceed 3.3V on any GPIO pin

3. **Soldering Safety:**
   - Use proper ventilation
   - Avoid overheating batteries
   - Check for short circuits before powering on

4. **Testing:**
   - Test each section independently
   - Use multimeter to verify connections
   - Check polarity before connecting batteries
   - Start with low currents when testing

## Troubleshooting

### ESP32 Not Powering On
- Check battery voltage (should be > 3.3V)
- Verify button connection
- Check VIN pin connection

### Voltage Reading Incorrect
- Verify voltage divider resistor values
- Check ADC pin connections
- Calibrate in code if necessary

### Servo Not Moving
- Check 3.3V power supply capacity
- Verify signal pin connection
- Test servo separately with known good circuit

### LCD Not Displaying
- Check I2C address (usually 0x27 or 0x3F)
- Verify SDA/SCL connections
- Check 3.3V power supply
- Adjust contrast potentiometer on LCD

### DHT11 Not Reading
- Verify data pin connection
- Check 3.3V power supply
- Ensure proper timing in code
- Try different GPIO pin if needed

## Recommended Tools

- Multimeter (essential)
- Soldering iron and solder
- Wire strippers
- Small screwdrivers
- Breadboard for prototyping
- Heat shrink tubing
- Electrical tape
- Cable ties for organization

## PCB Design Notes

For a permanent installation, consider designing a custom PCB with:
- ESP32 socket
- Voltage divider circuits
- Protection diodes
- Connector headers for all peripherals
- Mounting holes
- Proper trace widths for current capacity
- Ground plane for noise reduction
