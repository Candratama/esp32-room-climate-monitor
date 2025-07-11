# ESP32 Room Monitor - Wiring Schematic

## Component Connections

```
                 ┌─────────────────┐
                 │     ESP32       │
                 │                 │
            5V ──┤ 5V              │
           GND ──┤ GND             │
                 │                 │
                 │            GPIO4├── DE/RE (RS485)
                 │           GPIO16├── RO (RS485)
                 │           GPIO17├── DI (RS485)
                 │                 │
                 │           GPIO21├── SDA (OLED)
                 │           GPIO22├── SCL (OLED)
                 │          3.3V ──┤ ── VCC (OLED)
                 │           GND ──┤ ── GND (OLED)
                 └─────────────────┘
                         │
                         │
                 ┌───────▼─────────┐      ┌─────────────────┐
                 │  RS485 to TTL   │      │   0.96" OLED    │
                 │   Converter     │      │     Display     │
                 │                 │      │   (128x64 I2C)  │
            VCC ─┤ VCC        A+ ──┼──────┤                 │
            GND ─┤ GND        B- ──┼──┐   │   SDA ── GPIO21 │
         GPIO16 ─┤ RO             │  │   │   SCL ── GPIO22 │
         GPIO17 ─┤ DI             │  │   │   VCC ── 3.3V   │
          GPIO4 ─┤ DE/RE          │  │   │   GND ── GND    │
                 └─────────────────┘  │   └─────────────────┘
                                      │
                                      │
                              ┌───────▼─────────┐
                              │   XY-MD02       │
                              │ Temp/Humidity   │
                              │    Sensor       │
                              │                 │
                              │ A+ ── RS485 A+  │
                              │ B- ── RS485 B-  │
                              │ VCC ── 5-24V    │
                              │ GND ── GND      │
                              └─────────────────┘
```

## Power Requirements

- **ESP32**: 3.3V/5V via USB or external supply
- **OLED Display**: 3.3V (from ESP32)
- **RS485 Module**: 5V (from ESP32 5V pin)
- **XY-MD02 Sensor**: 5-24V DC (external power supply recommended)

## Pin Assignment Summary

| Function      | ESP32 Pin | Component Pin |
| ------------- | --------- | ------------- |
| RS485 Data RX | GPIO16    | RO            |
| RS485 Data TX | GPIO17    | DI            |
| RS485 Control | GPIO4     | DE & RE       |
| OLED Data     | GPIO21    | SDA           |
| OLED Clock    | GPIO22    | SCL           |

## Notes

1. **RS485 Termination**: The XY-MD02 sensor may need 120Ω termination resistors between A+ and B- if you experience communication issues.

2. **Power Supply**: For reliable operation, consider using an external 12V power supply for the XY-MD02 sensor, especially in industrial environments.

3. **Cable Length**: RS485 can handle up to 1200 meters cable length, making it ideal for remote sensor placement.

4. **Ground Isolation**: In noisy environments, consider using an isolated RS485 module to prevent ground loops.

## Testing Points

- **Serial Monitor**: Check ESP32 serial output at 115200 baud
- **OLED Display**: Should show temperature and humidity readings
- **RS485 Communication**: Use an oscilloscope to verify data transmission on A+/B- lines
- **Power Consumption**: Typical total system consumption ~100-200mA
