# HydroCharge - Pure Connections Guide

## BATTERY CONNECTIONS
```
12V Battery (+) → Terminal Block Positive Rail
12V Battery (-) → Terminal Block Negative Rail (Common Ground)
```

## DC-DC CONVERTER (12V → 5V)
```
VIN  → Terminal Block Positive Rail
GND  → Terminal Block Negative Rail
VOUT → 5V Power Rail
```

## NODEMCU ESP8266
```
Vin / 5V → 5V Power Rail (from DC-DC)
GND      → Common Ground
D5       → Ultrasonic Sensor TRIG
D6       → Ultrasonic Sensor ECHO (via level shifting)
D1       → Relay Module #1 IN (Motor Relay)
D2       → Relay Module #2 IN (USB Relay)
```

## RELAY MODULE #1 (Motor Control)
```
VCC → 5V Power Rail
GND → Common Ground
IN  → NodeMCU D1

COM → Terminal Block Positive (12V)
NO  → Split to both motors (+)
```

## RELAY MODULE #2 (USB Charging)
```
VCC → 5V Power Rail
GND → Common Ground
IN  → NodeMCU D2

COM → Terminal Block Positive (12V)
NO  → Female USB Breakout VIN
```

## ULTRASONIC SENSOR
```
VCC  → 5V Power Rail
GND  → Common Ground
TRIG → NodeMCU D5
ECHO → NodeMCU D6 (use a voltage divider or level shifter)
```

## MOTORS
```
12V Motor (+) → Relay #1 NO Contact
12V Motor (-) → Common Ground
```

## USB BREAKOUT BOARD
```
VIN → Relay #2 NO Contact
GND → Common Ground
D+  → USB Connector (pass-through)
D-  → USB Connector (pass-through)
```

## COMMON GROUND (All connected to one point)
```
Battery (-)
Terminal Block Negative Rail
NodeMCU GND
Relay #1 GND
Relay #2 GND
Sensor GND
Motor (-) terminals
USB Board GND
DC-DC Converter GND
```

## QUICK CONNECTION TABLE

| From | To | Wire |
|------|---|------|
| Battery (+) | T-Block Pos | Red |
| Battery (-) | T-Block Neg | Black |
| T-Block Pos | DC-DC VIN | Red |
| DC-DC GND | T-Block Neg | Black |
| DC-DC VOUT | 5V Rail | Orange |
| 5V Rail | NodeMCU Vin | Orange |
| 5V Rail | Relay #1 VCC | Orange |
| 5V Rail | Relay #2 VCC | Orange |
| 5V Rail | Sensor VCC | Orange |
| NodeMCU D5 | Sensor OUT | Yellow |
| NodeMCU D1 | Relay #1 IN | Yellow |
| NodeMCU D2 | Relay #2 IN | Yellow |
| T-Block Pos | Relay #1 COM | Red |
| Relay #1 NO | Motor 12V (+) | Red |
| Motor (-) | T-Block Neg | Black |
| T-Block Pos | Relay #2 COM | Red |
| Relay #2 NO | USB VIN | Red |
| USB GND | T-Block Neg | Black |
| All GND | Common Ground | Black |
