# ESP32 Room Climate Monitor

![PlatformIO CI](https://github.com/YOUR_USERNAME/esp32-room-climate-monitor/workflows/PlatformIO%20CI/badge.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-orange.svg)
![Framework](https://img.shields.io/badge/framework-Arduino-blue.svg)

A professional-grade room monitoring system using ESP32 with XY-MD02 temperature/humidity sensor, featuring clean code architecture and comprehensive documentation.

## 🌟 Key Features

- **Real-time Monitoring**: Temperature and humidity with 0.1° precision
- **Professional Display**: SSD1306 OLED with comfort status indicators
- **Robust Communication**: Modbus RTU over RS485 with CRC validation
- **Clean Architecture**: Modular, well-documented codebase
- **Error Handling**: Comprehensive validation and graceful degradation
- **Configurable**: Easy hardware and parameter customization

## 📷 Screenshots

*Add photos of your working setup here*

## 🔧 Hardware Requirements

| Component | Description | Notes |
|-----------|-------------|-------|
| ESP32 Development Board | Main microcontroller | Any ESP32 variant |
| XY-MD02 Sensor | Temperature/Humidity sensor | RS485 output |
| RS485 to TTL Module | Communication converter | Auto direction control |
| SSD1306 OLED | 128x64 display | I2C interface |
| Jumper Wires | Connections | - |
| Breadboard/PCB | Assembly | - |

## ⚡ Quick Start

1. **Clone the repository**
   ```bash
   git clone https://github.com/YOUR_USERNAME/esp32-room-climate-monitor.git
   cd esp32-room-climate-monitor
   ```

2. **Install PlatformIO**
   - Install [VS Code](https://code.visualstudio.com/)
   - Install [PlatformIO extension](https://platformio.org/install/ide?install=vscode)

3. **Wire the components** (see [Wiring Guide](docs/wiring_schematic.md))

4. **Build and upload**
   ```bash
   pio run --target upload
   pio device monitor
   ```

## 📋 Detailed Documentation

- [📖 Complete Setup Guide](README.md)
- [🔌 Wiring Schematic](docs/wiring_schematic.md)
- [🏗️ Code Architecture](docs/CODE_STRUCTURE.md)
- [⚙️ Configuration Guide](include/config.h)

## 🎯 Performance

- **Response Time**: ~20ms sensor reading
- **Memory Usage**: 6.7% RAM, 23.1% Flash
- **Update Rate**: 2-second sensor reads, 1-second display updates
- **Protocols**: Modbus RTU, I2C, Serial debugging

## 🛠️ Development

### Project Structure
```
esp32-room-climate-monitor/
├── src/main.cpp              # Main application
├── include/config.h          # Hardware configuration
├── docs/                     # Documentation
├── test/                     # Test utilities
├── .github/workflows/        # CI/CD pipeline
└── README.md                 # This file
```

### Building
```bash
# Build project
pio run

# Upload to ESP32
pio run --target upload

# Monitor serial output
pio device monitor --baud 115200

# Run tests
pio test
```

## 🤝 Contributing

Contributions are welcome! Please read our contributing guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📊 Monitoring Output

```
==========================================
ESP32 Room Climate Monitor v1.0
Initializing system components...
==========================================
RS485 initialized with automatic direction control
OLED display initialized successfully
System initialization complete!
Starting monitoring loop...
TX: 01 04 00 01 00 02 20 0B 
RX: 9 bytes after 20 ms
RX: 01 04 04 01 1D 02 7F 2A FE 
SUCCESS! Temperature: 28.5°C, Humidity: 63.9%
```

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| Sensor Error | Check RS485 wiring and power |
| OLED blank | Verify I2C connections and address |
| No serial output | Check USB connection and baud rate |
| Communication timeout | Verify sensor address and protocol |

## 📈 Future Roadmap

- [ ] WiFi connectivity and web interface
- [ ] Data logging to SD card
- [ ] Multiple sensor support
- [ ] Mobile app integration
- [ ] Cloud data synchronization
- [ ] Historical trend analysis

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- ESP32 community for excellent documentation
- Adafruit for the display libraries
- PlatformIO for the development platform

## ⭐ Star History

[![Star History Chart](https://api.star-history.com/svg?repos=YOUR_USERNAME/esp32-room-climate-monitor&type=Date)](https://star-history.com/#YOUR_USERNAME/esp32-room-climate-monitor&Date)

---

**Made with ❤️ for the IoT community**
