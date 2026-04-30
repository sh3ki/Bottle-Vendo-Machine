FINAL LIST OF COMPONENTS
Nodemcu Esp8266
12V motor
6V motor
GT Pulley
2 5V relay modules
Jumper wires
Octocupler
DC DC Buck Converter
Terminal Blocks
Infrared Proximity Sensor
12V lithium rechargeable battery
female usb breakout board


esp8266
12v motor
dc dc buck converter 
2 bestep JQC3F-05VDC-C relay module
Infrared Proximity Sensor
12V lithium rechargeable battery
female usb breakout board
toggle switch
12v battery
capacitor 4700uF 10v

---

## CURRENT CONNECTIONS (YOUR EXISTING SETUP - PROBLEMATIC)
```
NEGATIVE SIDE:
  Toggle Switch Black Terminal
    ↓
  Battery Negative
    ↓
  Terminal Block Negative Rail
    ├→ DC-DC Converter GND
    ├→ USB Breakout Board GND
    └→ Unknown wire with resistor (connects to positive - WRONG!)

POSITIVE SIDE (Series Configuration - CAUSES VOLTAGE DROP):
  Battery Positive (+)
    ↓
  Toggle Switch Red Terminal
    ↓
  Fuse Holder #1 → DC-DC Converter VIN
    ↓
  Fuse Holder #2 → USB Breakout Board VIN
    ↓
  Fuse Holder #3 → Unknown resistor connection
    ↓
  All in series = VOLTAGE DROP PROBLEM

MISSING CONNECTIONS:
  ✗ Relays not connected (VCC, GND, IN pins unused)
  ✗ Motors not connected to relay outputs
  ✗ ESP8266 not connected to relay control pins
  ✗ Sensor not connected to ESP8266
  ✗ No capacitor for power smoothing
```

**CRITICAL ISSUES WITH CURRENT SETUP:**
- ⚠️ **Series fuses cause voltage drop** - devices get < 12V
- ⚠️ **Unknown resistor connecting negative to positive = SHORT CIRCUIT RISK**
- ⚠️ **No relay control** - relays VCC, GND, IN pins are floating
- ⚠️ **Motor cannot turn on** - not connected to relay output
- ⚠️ **ESP8266 cannot control anything** - no connections to relay IN pins
- ⚠️ **Sensor not wired** - infrared sensor has no output to ESP8266
- ⚠️ **Poor ground distribution** - common ground point missing
- ⚠️ **No power smoothing** - capacitor not installed

---

## CORRECT CONNECTIONS (RECOMMENDED)

### MASTER POWER DISTRIBUTION
```
Battery (+12V) 
  ↓ (Through Toggle Switch)
  ├→ Capacitor 4700uF Positive (for power smoothing)
  ├→ Terminal Block Positive Rail
  │
Battery (-/GND)
  ↓ (Through Toggle Switch)
  ├→ Capacitor 4700uF Negative
  ├→ Terminal Block Negative Rail
  └→ COMMON GROUND POINT (one point for all grounds)
```

### POWER SMOOTHING CAPACITOR (4700uF 10V)
```
+ Terminal → Terminal Block Positive (12V)
- Terminal → Terminal Block Negative (GND)
Purpose: Prevents voltage spikes and smooths power delivery
```

### DC-DC CONVERTER (12V → 5V via USB)
```
VIN  → Terminal Block Positive (12V)
GND  → Terminal Block Negative (GND)
USB Ports (3x) → 5V Power Output

Current Usage:
- USB Port #1 → ESP8266 (via USB cable) ✓
- USB Port #2 → Available
- USB Port #3 → Available
```

**All 12V components powered directly from Terminal Block:**
- Relay #1 VCC → Terminal Block 12V
- Relay #2 VCC → Terminal Block 12V  
- Sensor VCC → Terminal Block 12V
- USB Breakout IN+ → Relay #2 NO (controlled charging)

### ESP8266 POWER
```
USB Cable → ESP8266 via USB micro/USB-C port (from DC-DC USB Port #1)
GND → Connected through USB cable common ground
```

**✓ Already connected correctly via USB**

### CONTROL OUTPUTS (ESP8266)
```
D1 (GPIO5)  → Relay #1 IN (Motor control)
D2 (GPIO4)  → Relay #2 IN (USB breakout control)
D5 (GPIO14) → Infrared Sensor OUT
```

### RELAY #1 (MOTOR CONTROL - 10-30VDC, 10A @ 28VDC)
```
VCC → Terminal Block Positive (12V) ✓ SAFE
GND → Terminal Block Negative (GND)
IN  → NodeMCU D1 (3.3V control signal)
COM → Terminal Block Positive (12V)
NO  → 12V Motor (+)
```

**✓ Can be powered directly from 12V Terminal Block**

### RELAY #2 (USB BREAKOUT CONTROL - 10-30VDC, 10A @ 28VDC)
```
VCC → Terminal Block Positive (12V) ✓ SAFE
GND → Terminal Block Negative (GND)
IN  → NodeMCU D2 (3.3V control signal)
COM → Terminal Block Positive (12V)
NO  → USB Breakout IN+ (for smart charging control)
```

**✓ Controls USB Charging Duration:**
- When D2 = HIGH: Relay closes, 12V flows to USB Breakout, charging starts
- When D2 = LOW: Relay opens, USB Breakout loses power, charging stops
- Software can set charging time limits

### INFRARED SENSOR (6-30V Input)
```
VCC → Terminal Block Positive (12V) ✓ SAFE
GND → Terminal Block Negative (GND)
OUT → NodeMCU D5
```

**✓ Can be powered directly from 12V Terminal Block**

### MOTORS
```
12V Motor (+) → Relay #1 NO Contact
12V Motor (-) → Common Ground
```

### USB BREAKOUT BOARD (High-Voltage Module - 6-32V Input, IN+ and IN-)
```
SAFE! This breakout board can handle 6-32V input and converts to USB 5V
Your 12V connection is CORRECT!

SMART CONNECTION (Via Relay #2 for control):
IN+  → Relay #2 NO Contact ✓ CONTROLLED
IN-  → Terminal Block Negative (GND) ✓ SAFE
USB Port → Device charging (internally regulated to 5V)
```

**✓ CONTROLLED VIA RELAY #2:**
- USB Breakout IN+ connects to Relay #2 NO (not direct 12V)
- When ESP8266 sets D2 HIGH → Relay #2 energizes → 12V flows to USB Breakout
- When ESP8266 sets D2 LOW → Relay #2 de-energizes → USB charging stops
- Allows you to control charging duration via software!

### ALL GROUNDS CONNECTED TO ONE POINT
```
✓ Battery (-)
✓ Terminal Block Negative Rail
✓ ESP8266 GND
✓ Relay #1 GND
✓ Relay #2 GND
✓ Sensor GND
✓ Motor (-) terminals
✓ USB Board GND
✓ DC-DC Converter GND
```

---

## WIRING CHECKLIST

**CRITICAL POWER:**
- [ ] 12V Battery (+) → Toggle Switch → Terminal Block Positive
- [ ] 12V Battery (-) → Toggle Switch → Terminal Block Negative
- [ ] DC-DC Converter VIN from Terminal Block Positive
- [ ] DC-DC Converter GND to Terminal Block Negative
- [ ] DC-DC USB Port #1 → ESP8266 (via USB cable) ✓ Already done
- [ ] Capacitor 4700uF across Terminal Block (+) and (-)

**ESP8266 & LOGIC:**
- [ ] ESP8266 5V via USB cable from DC-DC Port #1 ✓ Already done
- [ ] ESP8266 GND via USB cable return (common ground)
- [ ] ESP8266 D1 to Relay #1 IN
- [ ] ESP8266 D2 to Relay #2 IN
- [ ] ESP8266 D5 to Infrared Sensor OUT

**RELAYS & SWITCHES:**
- [ ] Relay #1 VCC to Terminal Block Positive (12V) ✓ UPDATED
- [ ] Relay #1 GND to Terminal Block Negative (GND)
- [ ] Relay #1 IN to ESP8266 D1
- [ ] Relay #1 COM to Terminal Block Positive (12V)
- [ ] Relay #1 NO to Motor 12V (+)
- [ ] Relay #2 VCC to Terminal Block Positive (12V) ✓ REQUIRED
- [ ] Relay #2 GND to Terminal Block Negative (GND)
- [ ] Relay #2 IN to ESP8266 D2
- [ ] Relay #2 COM to Terminal Block Positive (12V)
- [ ] Relay #2 NO to USB Breakout IN+ (required for charging control)

**USB CHARGING:**
- [ ] USB Breakout IN+ to Relay #2 NO (controlled)
- [ ] USB Breakout IN- to Terminal Block Negative (GND)
- [ ] USB Port outputs safe 5V for device charging (when Relay #2 is active)

**SENSORS & OUTPUTS:**
- [ ] Infrared Sensor VCC to Terminal Block Positive (12V)
- [ ] Infrared Sensor GND to Terminal Block Negative (GND)
- [ ] Infrared Sensor OUT to ESP8266 D5
- [ ] Motor (-) to Terminal Block Negative (GND)

**IMPORTANT NOTES:**
- Remove the series fuse configuration - use parallel terminal blocks instead
- REMOVE THE RESISTOR CONNECTING NEGATIVE TO POSITIVE - THIS IS A SHORT CIRCUIT
- All grounds must connect at a single point for proper return path
- Use terminal blocks for clean, reliable connections
- Use appropriate wire gauges: 12V power (12-14 AWG), 5V signals (18-22 AWG)

---

## STEP-BY-STEP REWIRING PROCEDURE

### STEP 1: Disconnect Everything (Safety First)
- [ ] Remove battery from circuit
- [ ] Wait 30 seconds for capacitors to discharge
- [ ] Remove ALL wires from current setup

### STEP 2: Prepare Terminal Blocks
- [ ] Set up Terminal Block for 12V Power (Positive Rail)
- [ ] Set up Terminal Block for Ground (Negative Rail)
- [ ] Make sure blocks are accessible

### STEP 3: Install Capacitor (Power Smoothing)
- [ ] Connect capacitor (+) to Terminal Block Positive
- [ ] Connect capacitor (-) to Terminal Block Negative
- **This protects your ESP8266 from power spikes**

### STEP 4: Connect Battery & Switch
- [ ] Battery (+) → Toggle Switch → Terminal Block Positive
- [ ] Battery (-) → Toggle Switch → Terminal Block Negative
- **Check: Terminal block now has 12V power when switch is ON**

### STEP 5: Connect DC-DC Converter
- [ ] DC-DC VIN → Terminal Block Positive (12V)
- [ ] DC-DC GND → Terminal Block Negative (GND)
- [ ] DC-DC USB Port #1 → ESP8266 (USB cable) ✓ Already connected
- **Check: Measure 5V at DC-DC USB ports with multimeter**

### STEP 6: Connect ESP8266
- [ ] ESP8266 already powered via USB cable from DC-DC ✓ DONE
- [ ] Verify USB cable connection is secure
- **Check: LED on ESP8266 lights up**

### STEP 7: Connect Relay #1 (Motor Control)
- [ ] Relay #1 VCC → Terminal Block Positive (12V)
- [ ] Relay #1 GND → Terminal Block Negative (GND)
- [ ] Relay #1 IN → ESP8266 D1
- [ ] Relay #1 COM → Terminal Block Positive (12V)
- [ ] Relay #1 NO → Motor 12V (+) pin

### STEP 8: Connect Relay #2 (USB Control - Controls Charging)
- [ ] Relay #2 VCC → Terminal Block Positive (12V)
- [ ] Relay #2 GND → Terminal Block Negative (GND)
- [ ] Relay #2 IN → ESP8266 D2
- [ ] Relay #2 COM → Terminal Block Positive (12V)
- [ ] Relay #2 NO → USB Breakout IN+ (controls when charging happens)
- **This lets ESP8266 D2 control USB charging duration**

### STEP 9: Connect USB Breakout Board (Powered via Relay #2)
- [ ] USB Breakout IN+ → Relay #2 NO (receives 12V when relay activates)
- [ ] USB Breakout IN- → Terminal Block Negative (GND)
- [ ] USB Port → Device charging cable

### STEP 10: Connect Sensor
- [ ] Sensor VCC → Terminal Block Positive (12V)
- [ ] Sensor GND → Terminal Block Negative (GND)
- [ ] Sensor OUT → ESP8266 D5

### STEP 11: Connect Motors
- [ ] Motor (-) → Terminal Block Negative (GND)
- [ ] 12V Motor (+) connects through Relay #1 NO

### STEP 12: Final Ground Check
- [ ] All GNDs connected to ONE terminal block negative point
- [ ] Verify continuity with multimeter

### STEP 13: Power Test
- [ ] Turn on toggle switch
- [ ] Check: LED on ESP8266 lights up ✓
- [ ] Check: Measure 12V at Terminal Block Positive
- [ ] Check: Measure 12V at Relay VCC pins
- [ ] Check: USB port provides power to connected device (when D2 relay is active)
- [ ] If all OK, proceed to test relay control

