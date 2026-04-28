To guarantee that electricity is only utilized when a bottle is genuinely present, the procedure is built around a Hard Trigger logic.

Detection: The intake is continuously observed by the Infrared Proximity Sensor. It notifies the NodeMCU ESP8266 when it finds a bottle.

Processing: The NodeMCU serves as the "brain," taking in the sensor signal and turning on the relay modules.

Mechanical Action: The 12V and 6V motors are activated by the first relay. The bottle is moved through the machine by these motors using a GT pulley system.



Power Management: The primary power source is a 12V lithium rechargeable battery. High-current distribution is ensured by Terminal Blocks and AWG 14 pigtail cabling, while the DC-DC Buck Converter reduces this voltage down to power the electronics safely.

Charging Reward: The Female USB Breakout Board is activated by the second relay after a bottle is processed, giving each bottle precisely two minutes of charging time.

Safety and Stability: The NodeMCU is shielded from electrical noise by the Optocoupler (integrated into the relay) and Common Ground cable, preventing the sensor from incorrectly triggering.