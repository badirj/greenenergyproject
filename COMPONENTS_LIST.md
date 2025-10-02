# Components List and Specifications

## Complete Bill of Materials (BOM)

### Power Generation Components

#### 1. Solar Panels
- **Quantity:** 2
- **Specifications:** 6V 100mA each
- **Configuration:** Series (12V total)
- **Type:** Polycrystalline or monocrystalline
- **Dimensions:** ~60mm x 60mm (typical)
- **Purpose:** Primary power generation
- **Estimated Cost:** $5-10 per panel

**Purchase Links:**
- Amazon: "6V 100mA Solar Panel"
- AliExpress: "Mini Solar Cell 6V"
- Local electronics stores

#### 2. Piezoelectric Disks
- **Quantity:** 6
- **Specifications:** 27mm or 35mm diameter
- **Voltage Output:** ~10-30V peak (depends on pressure)
- **Configuration:** Parallel
- **Type:** Buzzer elements (piezo disks)
- **Purpose:** Mechanical energy harvesting
- **Estimated Cost:** $1-2 per disk

**Purchase Links:**
- Amazon: "Piezo Disk Buzzer Element 27mm"
- AliExpress: "Piezoelectric Ceramic Disc"
- eBay: "Piezo Transducer"

### Energy Storage Components

#### 3. 18650 Li-ion Batteries
- **Quantity:** 2
- **Specifications:**
  - Voltage: 3.7V nominal (4.2V fully charged, 3.0V minimum)
  - Capacity: 3400mAh
  - Type: Protected ICR18650
  - Chemistry: Lithium-ion
- **Purpose:** Energy storage
- **Estimated Cost:** $5-8 per battery

**Recommended Brands:**
- Samsung 35E
- LG MJ1
- Panasonic NCR18650B
- Sony VTC6

⚠️ **Warning:** Only use batteries with built-in protection circuits!

#### 4. TP4056 Charging Modules
- **Quantity:** 2
- **Specifications:**
  - Input: 4.5V-8V (up to 8V safe)
  - Charging Current: 1A (adjustable via resistor)
  - Protection: Overcharge, over-discharge, short circuit
  - Type: TP4056 with protection IC
- **Purpose:** Battery charging and protection
- **Estimated Cost:** $1-2 per module

**Note:** Use TP4056 modules WITH DW01A protection IC

#### 5. Supercapacitor
- **Quantity:** 1
- **Specifications:**
  - Capacitance: 1F
  - Voltage: 2.5V or 2.7V
  - Type: EDLC (Electric Double Layer Capacitor)
- **Purpose:** Buffer for piezo voltage spikes
- **Estimated Cost:** $2-5

**Alternative:** 4.7F supercapacitor for better energy storage

### Microcontroller and Sensors

#### 6. ESP32 DevKit V1
- **Quantity:** 1
- **Specifications:**
  - Processor: Dual-core Xtensa 32-bit LX6
  - WiFi: 802.11 b/g/n
  - Bluetooth: BLE 4.2
  - GPIO: 30 pins
  - ADC: 12-bit, multiple channels
  - Operating Voltage: 3.3V (can be powered via 5V VIN)
- **Purpose:** Main controller
- **Estimated Cost:** $6-10

**Board Types:**
- ESP32-WROOM-32
- DOIT ESP32 DevKit V1
- NodeMCU-32S

#### 7. DHT11 Sensor
- **Quantity:** 1
- **Specifications:**
  - Temperature Range: 0-50°C (±2°C accuracy)
  - Humidity Range: 20-90% RH (±5% accuracy)
  - Operating Voltage: 3.3V-5V
  - Interface: Single-wire digital
- **Purpose:** Environmental monitoring
- **Estimated Cost:** $2-3

**Alternative:** DHT22 for better accuracy (±0.5°C, ±2% RH)

#### 8. 16x2 I2C LCD Display
- **Quantity:** 1
- **Specifications:**
  - Display: 16 characters x 2 lines
  - Interface: I2C (via PCF8574 backpack)
  - Operating Voltage: 5V (works with 3.3V)
  - I2C Address: Usually 0x27 or 0x3F
  - Backlight: Blue or green
- **Purpose:** Local status display
- **Estimated Cost:** $3-5

**Alternative:** OLED 0.96" I2C display for better visibility

#### 9. SG90 Servo Motor
- **Quantity:** 1
- **Specifications:**
  - Operating Voltage: 4.8V-6V (works with 3.3V at lower torque)
  - Rotation: 180° (0° to 180°)
  - Torque: 1.8 kg/cm at 4.8V
  - Speed: 0.1s/60° at 4.8V
  - Control: PWM (50Hz, 1-2ms pulse width)
- **Purpose:** Solar panel protection mechanism
- **Estimated Cost:** $2-4

**Note:** May need external 5V power supply for full torque

### Passive Components and Hardware

#### 10. Diodes
- **Quantity:** 8
- **Type:** 1N4007
- **Specifications:**
  - Voltage: 1000V PIV
  - Current: 1A forward current
  - Type: Rectifier diode
- **Purpose:** Rectification and reverse protection
- **Estimated Cost:** $0.10 per diode

**Usage:**
- 6 diodes for piezo rectifier bridges (3 pairs)
- 2 diodes for protection

#### 11. Resistors
- **10KΩ Resistors:** 3 pieces
  - 2 for voltage dividers
  - 1 for button pull-up
- **1KΩ Resistors:** 2 pieces
  - 2 for voltage dividers
- **Power Rating:** 1/4W (0.25W)
- **Tolerance:** ±5% or ±1%
- **Estimated Cost:** $0.05-0.10 per resistor

#### 12. ON/OFF Button
- **Quantity:** 1
- **Type:** Momentary push button
- **Specifications:**
  - Rating: 12V 50mA minimum
  - Type: SPST (Single Pole Single Throw)
  - Mounting: Panel mount or PCB mount
- **Purpose:** Power control
- **Estimated Cost:** $0.50-1

**Alternative:** Latching toggle switch for permanent ON/OFF

### Connection and Assembly Materials

#### 13. Jumper Wires
- **22 AWG stranded wire** (various colors)
- **Dupont jumper wires** (Male-Female, Male-Male)
- **Estimated quantity:** 50+ pieces

#### 14. Breadboard or PCB
- **Breadboard:** 830 tie-points for prototyping
- **OR Perfboard:** 5x7cm or 7x9cm for permanent assembly
- **OR Custom PCB:** Designed and fabricated

#### 15. Battery Holders
- **Quantity:** 2
- **Type:** 18650 single cell holder
- **With leads or PCB mount**

#### 16. Connectors
- **JST connectors** for batteries
- **Screw terminals** for power connections
- **Pin headers** for modular connections

#### 17. Enclosure
- **Plastic project box** (size depends on layout)
- **Suggested size:** 150mm x 100mm x 50mm
- **Mounting holes** for components
- **Ventilation holes** for heat dissipation

### Optional Components

#### 18. External Power Supply (for testing)
- **5V 2A USB power adapter**
- **For powering ESP32 during development**

#### 19. USB Cable
- **Micro USB cable**
- **For programming ESP32**

#### 20. Voltage Regulator (if needed)
- **AMS1117-3.3V** or **LM1117-3.3V**
- **For stable 3.3V supply if using 5V peripherals**

#### 21. Heat Shrink Tubing
- **Various sizes**
- **For insulating connections**

#### 22. Double-sided Tape / Mounting Tape
- **For securing components in enclosure**

## Cost Estimate

| Component | Quantity | Unit Price | Total |
|-----------|----------|------------|-------|
| Solar Panels (6V 100mA) | 2 | $7 | $14 |
| Piezo Disks | 6 | $1.50 | $9 |
| 18650 Batteries (3400mAh) | 2 | $6 | $12 |
| TP4056 Modules | 2 | $1.50 | $3 |
| Supercapacitor (1F) | 1 | $3 | $3 |
| ESP32 DevKit V1 | 1 | $8 | $8 |
| DHT11 Sensor | 1 | $2.50 | $2.50 |
| 16x2 I2C LCD | 1 | $4 | $4 |
| SG90 Servo Motor | 1 | $3 | $3 |
| 1N4007 Diodes | 8 | $0.10 | $0.80 |
| Resistors (mixed) | 5 | $0.10 | $0.50 |
| Push Button | 1 | $0.75 | $0.75 |
| Wires & Connectors | Set | $5 | $5 |
| Breadboard/PCB | 1 | $3 | $3 |
| Battery Holders | 2 | $1 | $2 |
| Enclosure | 1 | $5 | $5 |
| **TOTAL** | | | **~$75-80** |

## Where to Buy

### Online Retailers
1. **Amazon** - Fast shipping, good for prototyping
2. **AliExpress** - Cheap prices, slower shipping (2-4 weeks)
3. **eBay** - Good for individual components
4. **DigiKey** - High quality, fast shipping, more expensive
5. **Mouser Electronics** - Similar to DigiKey
6. **Adafruit** - Quality components, good support
7. **SparkFun** - Educational focus, quality parts

### Local Options
- Local electronics stores
- University electronics labs (if student)
- Maker spaces and hackerspaces
- Robotics clubs

## Component Alternatives

### Budget-Friendly Alternatives
1. **ESP32:** Use ESP8266 (cheaper, but no Bluetooth)
2. **LCD:** Use 4-bit parallel LCD (no I2C backpack needed)
3. **Batteries:** Use lower capacity 2200mAh cells
4. **Servo:** Use cheaper 9g micro servo
5. **Solar Panels:** Use salvaged panels from calculators or garden lights

### High-Performance Alternatives
1. **Solar Panels:** 12V 500mA panels for faster charging
2. **Batteries:** Samsung 35E (3500mAh) or LG MH1 (3200mAh high drain)
3. **Sensor:** BME280 for temperature, humidity, and pressure
4. **Display:** 1.3" OLED I2C for better visibility
5. **Servo:** MG90S (metal gear, more durable)

## Assembly Tools Required

### Essential Tools
- Soldering iron (40W or adjustable temperature)
- Solder (lead-free or 60/40)
- Wire strippers
- Wire cutters
- Multimeter (for testing voltages and continuity)
- Small screwdrivers (Phillips and flat head)
- USB cable for ESP32 programming

### Recommended Tools
- Helping hands/PCB holder
- Desoldering pump or wick
- Hot glue gun (for securing components)
- Drill with bits (for enclosure)
- Label maker (for wire identification)

## Component Testing Before Assembly

1. **Solar Panels:** Test voltage output in sunlight (should read ~6V each)
2. **Piezo Disks:** Tap each disk and measure voltage with oscilloscope
3. **Batteries:** Check voltage (should be 3.7V±0.2V)
4. **TP4056 Modules:** Test with dummy load before connecting batteries
5. **ESP32:** Upload test sketch to verify all pins work
6. **DHT11:** Test with example sketch
7. **LCD:** Test I2C address and display functionality
8. **Servo:** Test with Arduino servo sweep example

## Storage and Handling

### Battery Storage
- Store at 50-60% charge for long-term storage
- Keep in cool, dry place
- Check voltage monthly
- Never store fully discharged

### Component Storage
- Use anti-static bags for ESP32
- Keep solar panels protected from physical damage
- Store piezo disks away from moisture
- Label all components clearly

## Safety Equipment

- Safety glasses (for soldering and drilling)
- Well-ventilated workspace
- Fire extinguisher nearby
- First aid kit

## Recommended Spares

Keep extras of these components (they're cheap and may fail):
- 2 extra 1N4007 diodes
- 1 extra DHT11 sensor
- 1 extra servo motor
- 5 extra resistors (assorted values)
- Extra jumper wires

## Project Timeline

- **Component ordering:** 1-4 weeks (depending on shipping)
- **Testing individual components:** 2-3 days
- **Circuit assembly:** 2-3 days
- **Programming and debugging:** 2-4 days
- **Enclosure and final assembly:** 1-2 days
- **Testing and optimization:** 3-5 days

**Total estimated time:** 2-4 weeks from ordering to completion
