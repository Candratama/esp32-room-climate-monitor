# ESP32 Room Climate Monitor

A comprehensive room monitoring system using ESP32 with XY-MD02 temperature/humidity sensor, RS485 to TTL converter, and OLED display.

## Components Required

- **ESP32 Development Board**
- **XY-MD02 Temperature & Humidity Sensor** (RS485 output)
- **RS485 to TTL Converter Module**
- **0.96" OLED Display** (128x64, I2C interface)
- **Jumper wires**
- **Breadboard or PCB**

## Features

- Real-time temperature and humidity monitoring
- OLED display with clear readings
- RS485 Modbus RTU communication
- Error detection and status display
- Uptime counter
- Comfortable environment range indicators

## Wiring Diagram

### ESP32 to RS485 Module

```
ESP32 Pin    →    RS485 Module Pin
GPIO16 (RX2) →    RXD (Receive Data)
GPIO17 (TX2) →    TXD (Transmit Data)
5V           →    VCC
GND          →    GND
```

**Note**: This uses ESP32's Serial2 (Hardware Serial 2) with automatic direction control.

### ESP32 to OLED Display

```
ESP32 Pin    →    OLED Pin
GPIO21 (SDA) →    SDA
GPIO22 (SCL) →    SCL
3.3V         →    VCC
GND          →    GND
```

### RS485 Module to XY-MD02 Sensor

```
RS485 Module →    XY-MD02 Sensor
A+           →    A+
B-           →    B-
```

## Pin Configuration

| Component   | ESP32 Pin | Description                                         |
| ----------- | --------- | --------------------------------------------------- |
| RS485 RX    | GPIO16    | Data receive from sensor                            |
| RS485 TX    | GPIO17    | Data transmit to sensor                             |
| RS485 DE/RE | Not used  | Automatic direction control (if module supports it) |
| OLED SDA    | GPIO21    | I2C data line                                       |
| OLED SCL    | GPIO22    | I2C clock line                                      |

## Software Features

### Main Functions

- `readXYMD02Sensor()`: Reads temperature and humidity via Modbus RTU
- `updateDisplay()`: Updates OLED with current readings
- `calculateCRC()`: CRC calculation for Modbus communication

### Display Information

- Room temperature in Celsius
- Relative humidity percentage
- Connection status
- System uptime

## Modbus Communication

The XY-MD02 sensor uses Modbus RTU protocol:

- **Slave Address**: 0x01 (default)
- **Function Code**: 0x03 (Read Holding Registers)
- **Register Address**: 0x0000 (Temperature), 0x0001 (Humidity)
- **Baud Rate**: 9600
- **Data Format**: 8N1

### Command Frame

```
01 03 00 00 00 02 C4 0B
```

- `01`: Slave address
- `03`: Function code (Read Holding Registers)
- `00 00`: Starting register address
- `00 02`: Number of registers to read
- `C4 0B`: CRC checksum

## Installation

1. Install PlatformIO in VS Code
2. Clone this project
3. Connect components according to wiring diagram
4. Upload code to ESP32

```bash
# Build project
pio run

# Upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor
```

## Configuration

Edit `include/config.h` to modify:

- Pin assignments
- Sensor parameters
- Display settings
- Timing intervals
- Comfort range limits

## Troubleshooting

### Common Issues

1. **"Sensor Error" on display**

   - Check RS485 wiring (VCC, GND, TXD, RXD)
   - Verify sensor power supply (5-24V for XY-MD02)
   - Ensure correct baud rate (9600)
   - **For modules without DE/RE**: Make sure your RS485 module supports automatic direction control

2. **OLED not displaying**

   - Check I2C connections (SDA/SCL)
   - Verify OLED address (0x3C)
   - Check power supply to OLED (3.3V)

3. **No serial output**

   - Check USB connection
   - Verify correct COM port
   - Set monitor speed to 115200

4. **RS485 Communication Issues**
   - If your module has DE/RE pins but you're not using them, set `RS485_DE_PIN` to the appropriate GPIO in `config.h`
   - If communication is unreliable, try adding a small delay after sending commands
   - Check if your RS485 module requires 5V or 3.3V logic levels

### Serial Monitor Output

The system provides debug information via serial monitor:

```
ESP32 Room Monitor Starting...
Setup complete!
Temperature: 23.5°C, Humidity: 45.2%
Temperature: 23.6°C, Humidity: 45.1%
```

## Future Enhancements

- [ ] WiFi connectivity for remote monitoring
- [ ] Data logging to SD card
- [ ] Web interface for configuration
- [ ] Multiple sensor support
- [ ] Alert system for out-of-range values
- [ ] Historical data graphs

## License

This project is open source and available under the MIT License.

## Support

For issues and questions, please check the troubleshooting section or open an issue in the project repository.
