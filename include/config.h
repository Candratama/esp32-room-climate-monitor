/**
 * ESP32 Room Climate Monitor - Configuration File
 * 
 * This file contains all hardware pin assignments, sensor parameters,
 * and system configuration constants.
 * 
 * Modify these settings according to your hardware setup and requirements.
 * 
 * Author: Room Monitor System
 * Version: 1.0
 * Date: 2025
 */

#ifndef CONFIG_H
#define CONFIG_H

// ==================== PIN ASSIGNMENTS ====================

// RS485 Communication Pins
#define RS485_RX_PIN    16      // GPIO16 - Connect to RXD of RS485 module
#define RS485_TX_PIN    17      // GPIO17 - Connect to TXD of RS485 module  
#define RS485_DE_PIN    -1      // Direction control pin (-1 = auto control)
                                // If your RS485 module has DE/RE pins, 
                                // change this to the connected GPIO pin number

// I2C OLED Display Pins
#define OLED_SDA_PIN    21      // GPIO21 - OLED SDA (data line)
#define OLED_SCL_PIN    22      // GPIO22 - OLED SCL (clock line)

// ==================== DISPLAY CONFIGURATION ====================

// SSD1306 OLED Display Settings
#define SCREEN_WIDTH    128     // Display width in pixels
#define SCREEN_HEIGHT   64      // Display height in pixels
#define OLED_RESET      -1      // Reset pin (-1 = shared with ESP32 reset)
#define SCREEN_ADDRESS  0x3C    // I2C address (0x3C or 0x3D typically)

// ==================== SENSOR CONFIGURATION ====================

// XY-MD02 Temperature/Humidity Sensor Settings
#define SENSOR_ADDRESS      0x01    // Modbus device address (default: 0x01)
#define SENSOR_BAUD_RATE    9600    // Communication baud rate
#define SENSOR_TIMEOUT      1000    // Response timeout in milliseconds

// ==================== TIMING CONFIGURATION ====================

// System Update Intervals (in milliseconds)
#define SENSOR_READ_INTERVAL     2000   // Read sensor every 2 seconds
#define DISPLAY_UPDATE_INTERVAL  1000   // Update display every 1 second

// ==================== COMFORT ZONE THRESHOLDS ====================

// Temperature Comfort Range (in Celsius)
#define TEMP_MIN        18.0    // Minimum comfortable temperature (°C)
#define TEMP_MAX        26.0    // Maximum comfortable temperature (°C)

// Humidity Comfort Range (in percentage)
#define HUMIDITY_MIN    30.0    // Minimum comfortable humidity (%)
#define HUMIDITY_MAX    60.0    // Maximum comfortable humidity (%)

// ==================== END OF CONFIGURATION ====================

#endif // CONFIG_H
