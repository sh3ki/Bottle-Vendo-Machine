# HydroCharge Vendo Machine - Complete Wiring Guide

## Overview
This guide provides complete wiring instructions for the HydroCharge bottle vending machine system. The system uses a NodeMCU ESP8266 microcontroller to manage bottle detection, motor control, and USB charging rewards.

---

## Table of Contents
1. [Component Pin Reference](#component-pin-reference)
2. [Power Distribution](#power-distribution)
3. [Wiring Connections by Component](#wiring-connections-by-component)
4. [NodeMCU ESP8266 Pin Mapping](#nodemcu-esp8266-pin-mapping)
5. [Relay Module Configuration](#relay-module-configuration)
6. [Motor Control Circuit](#motor-control-circuit)
7. [Sensor Circuit](#sensor-circuit)
8. [USB Charging Circuit](#usb-charging-circuit)
9. [Safety & Grounding](#safety--grounding)
10. [Complete System Diagram](#complete-system-diagram)
11. [Troubleshooting Checklist](#troubleshooting-checklist)

---

## Component Pin Reference

### Main Components
- **NodeMCU ESP8266**: Microcontroller and brain of the system
- **12V Lithium Battery**: Primary power source (1200mAh typical)
- **DC-DC Buck Converter**: Steps down 12V to 5V for electronics
- **2× 5V Relay Modules**: Control motor and USB charging circuits
- **Infrared Proximity Sensor**: Detects bottle presence
- **12V Motor**: Primary drive motor
- **6V Motor**: Secondary drive motor with GT pulley
- **Female USB Breakout Board**: Provides USB charging port
- **Optocoupler**: Noise isolation (typically built into relay modules)
- **Terminal Blocks**: High-current power distribution

---

## Power Distribution

### Main Power Chain
```
12V Lithium Battery 
    ↓
Terminal Block (Main Bus)
    ├→ [HIGH CURRENT] 12V Motor + 6V Motor
    └→ DC-DC Buck Converter
        ↓
        5V Rail (Low Current)
        ├→ NodeMCU ESP8266
        ├→ 5V Relay Modules (2×)
        └→ Infrared Proximity Sensor
```

### Battery Specifications
- **Voltage**: 12V DC
- **Recommended Capacity**: 1200-2000mAh lithium
- **Usage**: All relay actuation, motor driving, and electronics power
- **Protection**: Built-in BMS (Battery Management System)

---

## Wiring Connections by Component

### 1. BATTERY TO POWER DISTRIBUTION

**From 12V Lithium Battery (+) Terminal:**
- Red wire → Terminal Block (POSITIVE Rail)
- Black wire → Terminal Block (NEGATIVE Rail) / Common Ground

**Terminal Block Positive Rail:**
- Output 1 → Relay Module #1 VCC (Voltage Common Collector)
- Output 2 → DC-DC Buck Converter VIN (Input)
- Output 3 → Motor Relay Circuit HIGH

**Terminal Block Negative Rail (Common Ground):**
- Connect to: All GND connections throughout the system

---

### 2. DC-DC BUCK CONVERTER (12V to 5V)

**Input Side (12V from Battery):**
- VIN (Red) → Terminal Block Positive Rail
- GND (Black) → Terminal Block Negative Rail / Common Ground

**Output Side (5V for Electronics):**
- VOUT (Red) → 5V Power Rail
- GND (Black) → Common Ground

**5V Power Rail connects to:**
- NodeMCU D1 (Vin pin, labeled 3V3 or 5V on some boards)
- Both Relay Modules VCC
- Infrared Sensor VCC (Positive)

---

### 3. NODEMCU ESP8266 - POWER CONNECTIONS

| Pin | Connection | Wire Color |
|-----|-----------|-----------|
| 5V / Vin | DC-DC Converter Output (5V) | Red |
| GND | Common Ground (Battery Negative) | Black |
| D5 | Infrared Sensor Signal | Yellow |
| D1 | Relay Module #1 IN (Motor Relay) | Orange |
| D2 | Relay Module #2 IN (USB Relay) | Purple |

**NodeMCU USB Port:**
- Leave disconnected or use only for Serial debugging (do NOT power from USB during operation)

---

### 4. RELAY MODULE #1 - MOTOR CONTROL RELAY

**Power Connections:**
| Pin | Connection | Details |
|-----|-----------|---------|
| VCC | 5V Rail (from DC-DC Converter) | Powers the relay coil |
| GND | Common Ground | Reference for relay logic |
| IN | NodeMCU D1 | Signal from microcontroller (HIGH = activate) |

**NO (Normally Open) Contacts - Motor Circuit:**
| Contact | Connection | Purpose |
|---------|-----------|---------|
| COM (Common) | Terminal Block 12V Positive | Main 12V power input |
| NO (Normally Open) | Motor Positive Lines | Routes 12V to motors |

**Motor Output Wiring:**
- NO Contact → Split to both motor positive terminals
- Motor Negative Terminals → Terminal Block Negative Rail / Common Ground

```
Motor Circuit:
12V Battery (+) → Terminal Block → Relay Module #1 (COM to NO)
                                        ↓
                    Split Signal
                    ├→ 12V Motor (+) terminal
                    └→ 6V Motor (+) terminal
                    
                    Both Motors (-) terminals → Common Ground
```

---

### 5. RELAY MODULE #2 - USB CHARGING RELAY

**Power Connections:**
| Pin | Connection | Details |
|-----|-----------|---------|
| VCC | 5V Rail | Powers the relay coil |
| GND | Common Ground | Reference for relay logic |
| IN | NodeMCU D2 | Signal from microcontroller (HIGH = activate) |

**NO (Normally Open) Contacts - USB Port Circuit:**
| Contact | Connection | Purpose |
|---------|-----------|---------|
| COM | 12V from Battery (via Relay Logic) | Main power for USB |
| NO | Female USB Breakout Board VIN | Supplies 5V USB charging |

**USB Breakout Board Connections:**
```
USB Breakout Board Pinout (Standard):
- VIN → Relay Module #2 NO Contact (active low)
- GND → Common Ground
- D+ → USB Port Data+
- D- → USB Port Data-
```

---

### 6. INFRARED PROXIMITY SENSOR

**Sensor Connection:**
| Pin | Connection | Wire Color | Voltage |
|-----|-----------|-----------|---------|
| VCC | 5V Rail | Red | +5V |
| GND | Common Ground | Black | Ground |
| OUT/Signal | NodeMCU D5 | Yellow | 3.3V Logic |

**Sensor Type:** Infrared Proximity Sensor (typically SR0001 or similar)
- **Detection Range**: ~2-8cm (adjustable potentiometer on module)
- **Logic Output**: Active LOW (sensor pulls to ground when object detected)
- **NodeMCU Pin D5 Configuration**: INPUT_PULLUP (enables internal pull-up resistor)

**Sensor Debounce Logic (from code):**
```
When object detected:
1. Sensor OUT goes LOW
2. 150ms delay for debounce
3. If still LOW, then object is confirmed
4. Trigger motor activation
```

---

## NodeMCU ESP8266 Pin Mapping

### Pinout Diagram (NodeMCU v3)
```
                    USB Port
                        │
        ┌───────────────────────────────┐
        │   NodeMCU ESP8266 - Front    │
    D0  │ ∘  RST                    TX ∘│ D10
    D1  │ ∘  ADC                    RX ∘│ D9
    D2  │ ∘  CH                     D8 ∘│ D8
    D3  │ ∘  CS                     D7 ∘│ D7
    D4  │ ∘  MOSI                   D6 ∘│ D6
    D5  │ ∘  MISO                   D5 ∘│ Infrared Sensor
    D6  │ ∘  SCLK                   D4 ∘│ D4
    D7  │ ∘  SD3                    D3 ∘│ D3
    D8  │ ∘  SD2                    D2 ∘│ USB Relay (D2)
        │
        │ GND ∘      ∘ 3V3 / 5V (Vin) │
        │ GND ∘      ∘ GND            │
        └───────────────────────────────┘
```

### Used Pins in HydroCharge System
| Pin | Function | Usage | Current Draw |
|-----|----------|-------|--------------|
| D5 | Infrared Sensor Input | Bottle Detection | ~5mA max |
| D1 | Motor Relay Signal | Drive Motor Control | ~40mA |
| D2 | USB Relay Signal | Charging Port Control | ~40mA |
| GND | Common Ground | All circuits | Reference |
| 3V3/5V Vin | Power Input | From DC-DC Converter | ~250mA max |

---

## Relay Module Configuration

### 5V Relay Module Specifications
- **Coil Voltage**: 5V DC
- **Coil Current**: ~30-50mA per relay
- **Contact Rating**: 10A @ 250V AC or 10A @ 30V DC
- **Contact Type**: NO (Normally Open), optional COM and NC

### Relay Module Terminal Layout
```
Relay Module #1 (Motor Control)
┌─────────────────┐
│  VCC  GND  IN   │ ← Power pins (5V side)
└─────────────────┘
│  COM  NO   NC   │ ← Contact pins (switch side)
└─────────────────┘

Relay Module #2 (USB Charging)
┌─────────────────┐
│  VCC  GND  IN   │ ← Power pins (5V side)
└─────────────────┘
│  COM  NO   NC   │ ← Contact pins (switch side)
└─────────────────┘
```

### Relay Activation Logic
```
NodeMCU Control:
- digitalWrite(RELAY_MOTOR, HIGH)  → Relay #1 energizes → Motors spin
- digitalWrite(RELAY_MOTOR, LOW)   → Relay #1 de-energizes → Motors stop
- digitalWrite(RELAY_QC3, HIGH)    → Relay #2 energizes → USB port active
- digitalWrite(RELAY_QC3, LOW)     → Relay #2 de-energizes → USB port off
```

---

## Motor Control Circuit

### Motor Circuit Overview
```
12V Battery (+)
    │
    └─→ Terminal Block
        │
        └─→ Relay #1 COM terminal
            │
            └─→ Relay #1 NO terminal (when activated by NodeMCU D1 HIGH)
                │
                ├─→ 12V Motor (+)
                │   12V Motor (-) → Common Ground
                │
                └─→ 6V Motor (+)  [Note: Gets 12V, internal regulator drops to 6V]
                    6V Motor (-) → Common Ground
```

### Motor Specifications
| Motor | Voltage | Current | Function |
|-------|---------|---------|----------|
| 12V Motor | 12V | ~2-3A | Primary drive |
| 6V Motor | 6V (nominal, 12V input) | ~1-2A | Secondary drive with GT pulley |

### Motor Control Timing (from code)
```c
const unsigned long MOTOR_SPIN_TIME = 3000; // 3 seconds rotation

When bottle detected:
1. Relay #1 activates (HIGH) → Motors spin
2. Wait 3000ms (3 seconds)
3. Relay #1 deactivates (LOW) → Motors stop
4. Bottle dispensed
```

### Current Requirements
- **12V Motor**: ~2.5A average
- **6V Motor**: ~1.5A average
- **Total Peak Motor Current**: ~4A (within 12V battery capability)
- **Battery Must Support**: Minimum 8-10A instantaneous draw

---

## Sensor Circuit

### Infrared Proximity Sensor Details
```
Sensor Module (SR0001 typical)
┌──────────────────┐
│  VCC  GND  OUT   │
└──────────────────┘

VCC   → 5V Rail (DC-DC Converter output)
GND   → Common Ground
OUT   → NodeMCU D5 (with INPUT_PULLUP enabled)
```

### Detection Logic Flow
```
Object NOT Detected:
  Sensor OUT → FLOAT (HIGH via pull-up)
  NodeMCU D5 reads HIGH
  
Object Detected:
  Sensor OUT → PULLED TO GROUND (LOW)
  NodeMCU D5 reads LOW
  Debounce wait: 150ms
  If still LOW → Bottle confirmed!
  Activation: digitalWrite(RELAY_MOTOR, HIGH)
```

### Sensor Adjustment
- Most proximity sensors have a potentiometer on the module
- **Clockwise rotation**: Increases detection range (~8cm)
- **Counter-clockwise rotation**: Decreases detection range (~2cm)
- **Recommended**: Set to ~4-5cm for reliable bottle detection without false triggers

---

## USB Charging Circuit

### USB Charging System
```
12V Battery (+)
    │
    └─→ Relay #2 COM terminal
        │
        └─→ Relay #2 NO terminal (when NodeMCU D2 = HIGH)
            │
            └─→ Female USB Breakout Board VIN
                │
                ├─→ USB Data+ (D+)
                ├─→ USB Data- (D-)
                ├─→ USB VBUS (5V out)
                └─→ USB GND → Common Ground
```

### USB Port Output
- **Voltage**: 5V DC (from relay NO contact, full battery power)
- **Max Current**: 2.5A (limited by female USB connector)
- **Charging Duration**: 120 seconds (2 minutes per bottle)

### Charging Reward System (from code)
```c
const unsigned long BOTTLE_REWARD = 120000; // 2 minutes in milliseconds

Charging Timeline:
- Bottle detected → Motor spins (3 sec)
- Relay #2 activates → USB port provides 5V
- Charging active for 120 seconds
- Timer decreases (displayed on dashboard)
- When timer reaches 0 → Relay #2 deactivates
```

### USB Breakout Board Pinout
```
Standard Female USB-A Breakout:
┌─────────────────┐
│  1   2   3   4  │ (from left)
└─────────────────┘

Pin 1 (VBUS, Red)   → From Relay #2 NO contact
Pin 2 (D-, White)   → Direct through to USB connector
Pin 3 (D+, Green)   → Direct through to USB connector
Pin 4 (GND, Black)  → Common Ground
```

---

## Safety & Grounding

### Common Ground (Critical!)
All negative connections must tie to the same point:
```
12V Battery (-)
    │
    └─→ Common Ground Rail (Terminal Block Negative)
        │
        ├→ All Motor GND connections
        ├→ DC-DC Converter GND
        ├→ NodeMCU GND
        ├→ Relay Module GND (both)
        ├→ Sensor GND
        └→ USB Board GND
```

### Wiring Best Practices
1. **Use AWG 14** for high-current motor lines (12V motor at 3A)
2. **Use AWG 20-24** for signal lines (5V relay triggers, sensor output)
3. **Use AWG 16-18** for relay module power connections
4. **Color Code**:
   - Red = 12V Power
   - Orange = 5V Power from DC-DC
   - Yellow = 5V Signal lines
   - Black = All Ground connections
5. **Avoid Ground Loops**: Single point connection for all grounds

### Fuse/Protection
- **Recommended Main Fuse**: 10A inline fuse on 12V battery positive
- **Location**: Between battery and terminal block
- **Purpose**: Protects against short circuits and over-current

### Noise Isolation (Optocoupler)
- Most relay modules include built-in optocoupling between the coil and contacts
- Prevents high-current motor switching noise from affecting sensor circuit
- No additional wiring needed; already integrated in relay modules

---

## Complete System Diagram

### Full Wiring Schematic

```
                    ┌─────────────────────────────┐
                    │  12V Lithium Battery        │
                    │  (+) ──────────── (-)       │
                    └────────┬──────────┬──────────┘
                             │          │
                    ┌────────▼──────────▼────────┐
                    │   Terminal Block (T-Block) │
                    │ Positive Rail │ Negative   │
                    │   (12V +)     │  Rail (GND)│
                    └────────┬──────────┬────────┘
                             │          │
                   ┌─────────┴──────────┴────────────┐
                   │                                  │
                   ▼                                  ▼
    ┌──────────────────────────┐      ┌──────────────────────────┐
    │ DC-DC Buck Converter     │      │ Relay Module #1          │
    │ VIN: 12V                 │      │ VCC: 5V | GND: Ground    │
    │ VOUT: 5V                 │      │ IN: NodeMCU D1           │
    │                          │      │ COM: 12V from T-Block    │
    └────────────┬─────────────┘      │ NO: to Motors (split)    │
                 │                    └──────────────────────────┘
                 │ (5V Rail)                    │ Split
                 │                             │
        ┌────────┼──────────────┐      ┌───────┴──────────┐
        │        │              │      │                  │
        │        │              │      ▼                  ▼
        ▼        ▼              ▼   ┌────────┐        ┌────────┐
    ┌────────┐ ┌────────┐ ┌────────┐│ 12V    │        │ 6V     │
    │NodeMCU │ │Relay #1│ │Relay #2│ Motor  │        │ Motor  │
    │ESP8266 │ │ VCC/GND│ │ VCC/GND│        │        │        │
    │        │ └────────┘ └────────┘└───┬────┘        └───┬────┘
    │ D5 ◄──┼─────────────────────────────┐                │
    │(Sens) │   ┌──────────────────────┐  │                │
    │ D1 ────┼──→│ Relay #2 IN         │  │                │
    │(Relay1)│   │ (USB Charging       │  │                │
    │ D2 ────┼──→│  Control)           │  │                │
    │(Relay2)│   └──────────────────────┘  │                │
    │        │         ┌──────────────────┐│                │
    │ GND ───┼────────→│ Common Ground    ││                │
    │        │         │ (All connections)││                │
    └────────┘         └──────────────────┘│                │
         │                                  │                │
         │  ┌──────────────────────────┐   │                │
         │  │Infrared Sensor (D5 input)│   │                │
         │  │ VCC: 5V | OUT: to D5     │   │                │
         │  │ GND: Common              │   │                │
         │  └──────────────────────────┘   │                │
         │                                  │                │
         │  ┌──────────────────────────┐   │                │
         └─→│ Relay Module #2 (USB)    │◄──┴────────────────┘
            │ NO: to USB VIN           │
            └──────────────────────────┘
                     │
                     ▼
            ┌──────────────────────┐
            │ Female USB Breakout  │
            │ Board                │
            │ VIN: from Relay #2   │
            │ GND: Common          │
            │ D+, D- : Pass-through│
            └──────────────────────┘
                     │
                     ▼
            [USB Charging Port]
```

### Simplified Connection Table

| From | To | Wire Color | Gauge | Amperage |
|------|---|-----------|-------|----------|
| Battery (+) | Terminal Block Pos | Red | 14 | 10A |
| Battery (-) | Terminal Block Neg | Black | 14 | 10A |
| T-Block Pos | DC-DC VIN | Red | 16 | 5A |
| DC-DC VOUT | 5V Rail | Orange | 20 | 2A |
| 5V Rail | NodeMCU Vin | Orange | 20 | 500mA |
| 5V Rail | Relay #1 VCC | Orange | 20 | 500mA |
| 5V Rail | Relay #2 VCC | Orange | 20 | 500mA |
| 5V Rail | Sensor VCC | Orange | 24 | 50mA |
| NodeMCU D5 | Sensor OUT | Yellow | 24 | 5mA |
| NodeMCU D1 | Relay #1 IN | Yellow | 24 | 40mA |
| NodeMCU D2 | Relay #2 IN | Yellow | 24 | 40mA |
| T-Block Pos | Relay #1 COM | Red | 14 | 10A |
| Relay #1 NO | Motor 12V (+) | Red | 14 | 3A |
| Relay #1 NO | Motor 6V (+) | Red | 14 | 2A |
| Motor (-) | T-Block Neg | Black | 14 | 3A |
| T-Block Pos | Relay #2 COM | Red | 14 | 10A |
| Relay #2 NO | USB VIN | Red | 14 | 2.5A |
| USB GND | T-Block Neg | Black | 16 | 2.5A |

---

## Reconnection Checklist

Follow this step-by-step to reconnect your system:

### Phase 1: Power Distribution
- [ ] Connect 12V Battery (+) to Terminal Block Positive Rail (Red wire, AWG 14)
- [ ] Connect 12V Battery (-) to Terminal Block Negative Rail (Black wire, AWG 14)
- [ ] Connect DC-DC Converter VIN to Terminal Block Positive
- [ ] Connect DC-DC Converter GND to Terminal Block Negative
- [ ] Verify DC-DC Converter outputs 5V (multimeter check)

### Phase 2: Control Electronics
- [ ] Connect DC-DC VOUT (5V) to 5V Power Rail
- [ ] Connect 5V Rail to NodeMCU Vin pin
- [ ] Connect NodeMCU GND to Common Ground
- [ ] Connect Relay #1 VCC to 5V Rail, GND to Common Ground
- [ ] Connect Relay #2 VCC to 5V Rail, GND to Common Ground
- [ ] Connect Infrared Sensor VCC to 5V Rail, GND to Common Ground

### Phase 3: Signal Lines
- [ ] Connect Infrared Sensor OUT to NodeMCU D5
- [ ] Connect Relay #1 IN to NodeMCU D1
- [ ] Connect Relay #2 IN to NodeMCU D2

### Phase 4: Motor Circuit
- [ ] Connect Terminal Block Positive to Relay #1 COM terminal
- [ ] Connect Relay #1 NO terminal to 12V Motor (+)
- [ ] Connect Relay #1 NO terminal to 6V Motor (+) [same NO contact, split]
- [ ] Connect both Motor (-) terminals to Terminal Block Negative
- [ ] Test: Power up system, check if motors spin when bottle is detected

### Phase 5: USB Charging Circuit
- [ ] Connect Terminal Block Positive to Relay #2 COM terminal
- [ ] Connect Relay #2 NO terminal to Female USB Breakout VIN
- [ ] Connect USB Breakout GND to Terminal Block Negative
- [ ] Pass-through D+ and D- wires to USB connector (optional for charging)
- [ ] Test: Check if USB port provides 5V when relay activates

### Phase 6: Verification
- [ ] Power up system with battery
- [ ] Check web dashboard: `192.168.4.1` (HydroCharge network)
- [ ] Place bottle near sensor and verify detection
- [ ] Confirm motors activate for 3 seconds
- [ ] Confirm USB port provides 5V for 120 seconds after activation
- [ ] Monitor system for 5 minutes to ensure stability

---

## Troubleshooting Checklist

### System Won't Power On
- [ ] Check battery voltage (should read 12V on multimeter)
- [ ] Verify battery connections are tight
- [ ] Check fuse (if installed) - should not be blown
- [ ] Test DC-DC Converter output (should be 5V)

### Motors Don't Spin When Bottle Detected
- [ ] Verify sensor detects bottle (check web dashboard)
- [ ] Test sensor manually: wave hand over sensor, check D5 voltage changes
- [ ] Confirm relay #1 clicks when motor should activate
- [ ] Check motor power connections to relay #1 NO terminal
- [ ] Test motors directly from battery to confirm they work

### Sensor Not Detecting Bottles
- [ ] Adjust sensor potentiometer (try both directions)
- [ ] Check sensor VCC and GND connections
- [ ] Verify sensor OUT wire goes to NodeMCU D5
- [ ] Test sensor voltage: should be 5V on VCC, 0V on GND
- [ ] Clean sensor lens (IR sensor window)

### USB Port Not Charging
- [ ] Verify relay #2 clicks after bottle is processed
- [ ] Test USB voltage: should read 5V when relay is active
- [ ] Check USB breakout board connections
- [ ] Confirm relay #2 activates according to timer (120 sec)
- [ ] Try different device on USB port to rule out device issue

### Dashboard Not Accessible
- [ ] Connect to WiFi network: "HydroCharge_Vendo"
- [ ] Password: "hydrocharge"
- [ ] Navigate to: `192.168.4.1`
- [ ] Check NodeMCU power connection
- [ ] Verify NodeMCU is not in bootloop (check Serial at 115200 baud)

### Intermittent Sensor Triggers
- [ ] Check for electromagnetic interference from motors
- [ ] Verify common ground connections are solid
- [ ] Confirm sensor debounce working (150ms delay in code)
- [ ] Optocoupler in relays should isolate noise
- [ ] Try repositioning sensor away from motor wires

### Motors Spin But Won't Stop
- [ ] Verify relay #1 signal wire (D1) is connected
- [ ] Check relay #1 contact continuity
- [ ] Confirm motor spin timeout is reached (3000ms)
- [ ] Test relay manually: disconnect D1 signal, relay should stop clicking

---

## Electrical Specifications Summary

### System Voltage Levels
| Section | Voltage | Current | Notes |
|---------|---------|---------|-------|
| Battery | 12V DC | Up to 10A | Primary power source |
| Motor Circuit | 12V DC | 4A peak | High-current direct from battery |
| Relay Coil | 5V DC | 50mA each | Isolated from high-current |
| Control Logic (NodeMCU) | 5V DC | 250mA | From DC-DC converter |
| Signal Lines | 3.3V-5V logic | <100mA | ESP8266 logic levels |
| USB Output | 5V DC | 2.5A | For device charging |

### Wire Gauge Selection
- **AWG 14**: 12V motor power, battery connections (supports 15A)
- **AWG 16**: Relay module power, high-current connections (supports 10A)
- **AWG 18**: DC-DC converter, general power rails (supports 7A)
- **AWG 20-24**: Signal lines, sensor connections (supports <1A)

### Fusing Recommendation
- **Main Line Fuse**: 10A inline fuse between battery and terminal block
- **Motor Circuit**: No additional fuse (protected by relay)
- **USB Circuit**: No additional fuse (limited by USB specification)

---

## Notes & Tips

1. **Testing Before Full Assembly**: Use a multimeter to verify all voltage levels before connecting to the NodeMCU
2. **Relay Click Confirmation**: You should hear a distinct "click" sound when relays activate - this confirms signal is being received
3. **Motor Behavior**: Motors will spin for exactly 3 seconds when bottle is detected, controlled by `MOTOR_SPIN_TIME` in code
4. **Charging Duration**: Each bottle grants exactly 2 minutes (120 seconds) of charging time to the USB port
5. **WiFi SSID & Password**: Can be changed in the code, but default is:
   - Network: "HydroCharge_Vendo"
   - Password: "hydrocharge"
6. **Dashboard Refresh**: Web dashboard auto-refreshes every 2 seconds to show real-time status

---

## Emergency Disconnect

In case of emergency or malfunction:
1. Immediately disconnect the 12V battery's positive terminal
2. This will cut power to all systems (relays will deactivate)
3. All motors will stop, charging port will deactivate
4. Verify system is fully de-energized before troubleshooting

---

## Document Version
- **Version**: 1.0
- **Last Updated**: April 28, 2026
- **System**: HydroCharge Bottle Vending Machine
- **Status**: Fully Operational (Post-Reconnection Guide)
