/**
 * ESP32 Room Climate Monitor
 * 
 * Monitors temperature and humidity using XY-MD02 sensor via RS485
 * Displays real-time data on SSD1306 OLED with comfort status
 * 
 * Hardware Requirements:
 * - ESP32 Development Board
 * - XY-MD02 Temperature/Humidity Sensor
 * - RS485 to TTL Converter Module
 * - SSD1306 128x64 OLED Display
 * 
 * Author: Room Monitor System
 * Version: 1.0
 * Date: 2025
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>
#include "config.h"

// ==================== FUNCTION DECLARATIONS ====================
void initializeHardware();
void initializeRS485Communication();
void initializeOLEDDisplay();
void readXYMD02Sensor();
void updateDisplay();
void displaySensorData();
void displayErrorMessage();
void displayComfortStatus();
void displayUptime();
uint16_t calculateCRC(uint8_t *data, uint8_t length);
bool validateModbusResponse(uint8_t *response, uint8_t expectedLength);
void clearSerialBuffer();

// ==================== HARDWARE CONFIGURATION ====================
// OLED Display instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ==================== GLOBAL VARIABLES ====================
// Sensor data storage
float temperature = 0.0;        // Current temperature reading in Celsius
float humidity = 0.0;           // Current humidity reading in percentage
bool sensorConnected = false;   // Flag indicating sensor connection status

// Timing control variables
unsigned long lastSensorRead = 0;      // Timestamp of last sensor reading
unsigned long lastDisplayUpdate = 0;   // Timestamp of last display update

// ==================== MAIN SETUP FUNCTION ====================
/**
 * Initialize system components and hardware interfaces
 */
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("==========================================");
  Serial.println("ESP32 Room Climate Monitor v1.0");
  Serial.println("Initializing system components...");
  Serial.println("==========================================");
  
  // Initialize all hardware components
  initializeRS485Communication();
  initializeOLEDDisplay();
  
  Serial.println("System initialization complete!");
  Serial.println("Starting monitoring loop...");
}

// ==================== MAIN LOOP FUNCTION ====================
/**
 * Main execution loop - handles sensor reading and display updates
 */
void loop() {
  unsigned long currentTime = millis();
  
  // Read sensor data at specified intervals
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readXYMD02Sensor();
    lastSensorRead = currentTime;
  }
  
  // Update display at specified intervals
  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    updateDisplay();
    lastDisplayUpdate = currentTime;
  }
  
  // Small delay to prevent excessive CPU usage
  delay(100);
}

// ==================== HARDWARE INITIALIZATION ====================

/**
 * Initialize RS485 communication interface
 * Configures Serial2 for communication with XY-MD02 sensor
 */
void initializeRS485Communication() {
  Serial.println("Initializing RS485 communication...");
  
  // Configure Serial2 for RS485 communication
  Serial2.begin(SENSOR_BAUD_RATE, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
  
  // Configure direction control pin if defined
  if (RS485_DE_PIN >= 0) {
    pinMode(RS485_DE_PIN, OUTPUT);
    digitalWrite(RS485_DE_PIN, LOW); // Set to receive mode initially
    Serial.println("RS485 initialized with manual direction control");
  } else {
    Serial.println("RS485 initialized with automatic direction control");
  }
}

/**
 * Initialize OLED display
 * Sets up I2C communication and display parameters
 */
void initializeOLEDDisplay() {
  Serial.println("Initializing OLED display...");
  
  // Initialize I2C communication for OLED
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  
  // Initialize OLED display driver
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("ERROR: SSD1306 display allocation failed!"));
    Serial.println(F("Check wiring and I2C address"));
    for(;;); // Stop execution if display initialization fails
  }
  
  // Display startup message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Room Climate Monitor"));
  display.println(F("v1.0"));
  display.println();
  display.println(F("Initializing..."));
  display.println(F("Starting sensors..."));
  display.display();
  
  Serial.println("OLED display initialized successfully");
  delay(2000); // Show startup message
}

// ==================== SENSOR COMMUNICATION ====================

/**
 * Read temperature and humidity data from XY-MD02 sensor
 * Uses Modbus RTU protocol over RS485
 * Function Code: 0x04 (Read Input Registers)
 * Register Address: 0x0001
 * Quantity: 2 registers (temperature + humidity)
 */
void readXYMD02Sensor() {
  // Prepare Modbus RTU command for XY-MD02
  // Format: [DeviceID][Function][RegAddr_Hi][RegAddr_Lo][Quantity_Hi][Quantity_Lo][CRC_Lo][CRC_Hi]
  uint8_t command[8] = {
    SENSOR_ADDRESS,  // Device address (0x01)
    0x04,           // Function code: Read Input Registers
    0x00,           // Register address high byte (0x0001)
    0x01,           // Register address low byte
    0x00,           // Quantity high byte (2 registers)
    0x02,           // Quantity low byte
    0x00,           // CRC low byte (calculated below)
    0x00            // CRC high byte (calculated below)
  };
  
  // Calculate and append CRC for message integrity
  uint16_t crc = calculateCRC(command, 6);
  command[6] = crc & 0xFF;        // CRC low byte
  command[7] = (crc >> 8) & 0xFF; // CRC high byte
  
  // Debug: Print command being sent
  Serial.print("TX: ");
  for (int i = 0; i < 8; i++) {
    Serial.printf("%02X ", command[i]);
  }
  Serial.println();
  
  // Set RS485 to transmit mode (if direction control pin is configured)
  if (RS485_DE_PIN >= 0) {
    digitalWrite(RS485_DE_PIN, HIGH);
    delay(1); // Small delay for signal stabilization
  }
  
  // Transmit command to sensor
  Serial2.write(command, sizeof(command));
  Serial2.flush(); // Ensure all data is transmitted
  
  // Set RS485 to receive mode (if direction control pin is configured)
  if (RS485_DE_PIN >= 0) {
    digitalWrite(RS485_DE_PIN, LOW);
    delay(1); // Small delay for signal stabilization
  }
  
  // Wait for response from sensor
  // Expected response: 9 bytes total
  // Format: [DeviceID][Function][ByteCount][Data1_Hi][Data1_Lo][Data2_Hi][Data2_Lo][CRC_Lo][CRC_Hi]
  const uint8_t expectedResponseLength = 9;
  unsigned long startTime = millis();
  
  while (Serial2.available() < expectedResponseLength && 
         millis() - startTime < SENSOR_TIMEOUT) {
    delay(1); // Small delay in polling loop
  }
  
  Serial.printf("RX: %d bytes after %lu ms\n", 
                Serial2.available(), millis() - startTime);
  
  // Process received response
  if (Serial2.available() >= expectedResponseLength) {
    uint8_t response[expectedResponseLength];
    Serial2.readBytes(response, expectedResponseLength);
    
    // Debug: Print received response
    Serial.print("RX: ");
    for (int i = 0; i < expectedResponseLength; i++) {
      Serial.printf("%02X ", response[i]);
    }
    Serial.println();
    
    // Validate and parse response
    if (validateModbusResponse(response, expectedResponseLength)) {
      // Parse temperature data (registers are 16-bit big-endian, scaled by 10)
      uint16_t tempRaw = (response[3] << 8) | response[4];
      temperature = tempRaw / 10.0;
      
      // Parse humidity data (registers are 16-bit big-endian, scaled by 10)
      uint16_t humRaw = (response[5] << 8) | response[6];
      humidity = humRaw / 10.0;
      
      sensorConnected = true;
      
      Serial.printf("SUCCESS! Temperature: %.1f°C, Humidity: %.1f%%\n", 
                   temperature, humidity);
    } else {
      sensorConnected = false;
      Serial.println("ERROR: Invalid sensor response");
    }
  } else if (Serial2.available() > 0) {
    // Handle partial or error responses
    uint8_t partialResponse[20];
    int bytesRead = Serial2.readBytes(partialResponse, Serial2.available());
    
    Serial.print("Partial response: ");
    for (int i = 0; i < bytesRead; i++) {
      Serial.printf("%02X ", partialResponse[i]);
    }
    Serial.println();
    
    // Check for Modbus exception response
    if (bytesRead >= 3 && partialResponse[1] & 0x80) {
      Serial.printf("Modbus Exception - Function: %02X, Code: %02X\n", 
                   partialResponse[1] & 0x7F, partialResponse[2]);
    }
    sensorConnected = false;
  } else {
    Serial.println("ERROR: No response from XY-MD02 sensor (timeout)");
    sensorConnected = false;
  }
  
  // Clear any remaining bytes in receive buffer
  clearSerialBuffer();
}

// ==================== DISPLAY FUNCTIONS ====================

/**
 * Main display update function
 * Orchestrates the complete display update cycle
 */
void updateDisplay() {
  display.clearDisplay();
  
  // Display title header
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("ROOM"));
  display.println(F("MONITOR"));
  
  // Reset text size for data display
  display.setTextSize(1);
  
  // Display appropriate content based on sensor status
  if (sensorConnected) {
    displaySensorData();
    displayComfortStatus();
  } else {
    displayErrorMessage();
  }
  
  // Always show system uptime
  displayUptime();
  
  // Update the physical display
  display.display();
}

/**
 * Display current sensor readings with warning indicators
 */
void displaySensorData() {
  // Display temperature with out-of-range warning
  display.setCursor(0, 32);
  display.printf("Temp: %.1f C", temperature);
  if (temperature < TEMP_MIN || temperature > TEMP_MAX) {
    display.print(" !"); // Warning indicator for temperature
  }
  
  // Display humidity with out-of-range warning
  display.setCursor(0, 42);
  display.printf("Humidity: %.1f%%", humidity);
  if (humidity < HUMIDITY_MIN || humidity > HUMIDITY_MAX) {
    display.print(" !"); // Warning indicator for humidity
  }
}

/**
 * Display error message when sensor is not connected
 */
void displayErrorMessage() {
  display.setCursor(0, 35);
  display.println("Sensor Error!");
  display.setCursor(0, 45);
  display.println("Check Connection");
}

/**
 * Display comfort status based on temperature and humidity ranges
 */
void displayComfortStatus() {
  display.setCursor(0, 52);
  
  // Determine comfort status based on both temperature and humidity
  bool tempOK = (temperature >= TEMP_MIN && temperature <= TEMP_MAX);
  bool humidityOK = (humidity >= HUMIDITY_MIN && humidity <= HUMIDITY_MAX);
  
  if (tempOK && humidityOK) {
    display.println("Status: COMFORT");
  } else {
    // Prioritize temperature issues over humidity issues in display
    if (temperature < TEMP_MIN) {
      display.println("Status: TOO COLD");
    } else if (temperature > TEMP_MAX) {
      display.println("Status: TOO HOT");
    } else if (humidity < HUMIDITY_MIN) {
      display.println("Status: TOO DRY");
    } else if (humidity > HUMIDITY_MAX) {
      display.println("Status: TOO HUMID");
    } else {
      display.println("Status: CHECK");
    }
  }
}

/**
 * Display system uptime in top-right corner
 */
void displayUptime() {
  display.setTextSize(1);
  
  // Calculate uptime and format string
  char uptimeStr[16];
  unsigned long uptimeSeconds = millis() / 1000;
  snprintf(uptimeStr, sizeof(uptimeStr), "Up: %lus", uptimeSeconds);
  
  // Calculate text bounds for right alignment
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(uptimeStr, 0, 0, &x1, &y1, &w, &h);
  
  // Position text with 2px padding from right edge
  display.setCursor(SCREEN_WIDTH - w - 2, 0);
  display.print(uptimeStr);
}

// ==================== UTILITY FUNCTIONS ====================

/**
 * Validate Modbus RTU response from XY-MD02 sensor
 * 
 * @param response Pointer to response data array
 * @param expectedLength Expected length of valid response
 * @return true if response is valid, false otherwise
 */
bool validateModbusResponse(uint8_t *response, uint8_t expectedLength) {
  // Verify response length
  if (expectedLength < 5) {
    Serial.println("ERROR: Response too short for validation");
    return false;
  }
  
  // Verify device address and function code
  // Expected format: [DeviceID][Function][ByteCount][Data...][CRC]
  if (response[0] != SENSOR_ADDRESS) {
    Serial.printf("ERROR: Wrong device address - Expected: %02X, Got: %02X\n", 
                 SENSOR_ADDRESS, response[0]);
    return false;
  }
  
  if (response[1] != 0x04) {
    Serial.printf("ERROR: Wrong function code - Expected: 04, Got: %02X\n", 
                 response[1]);
    return false;
  }
  
  if (response[2] != 0x04) {
    Serial.printf("ERROR: Wrong byte count - Expected: 04, Got: %02X\n", 
                 response[2]);
    return false;
  }
  
  // Verify CRC integrity
  uint16_t receivedCRC = response[expectedLength-2] | (response[expectedLength-1] << 8);
  uint16_t calculatedCRC = calculateCRC(response, expectedLength - 2);
  
  if (receivedCRC != calculatedCRC) {
    Serial.printf("ERROR: CRC mismatch - Received: %04X, Calculated: %04X\n", 
                 receivedCRC, calculatedCRC);
    return false;
  }
  
  return true;
}

/**
 * Clear all remaining bytes from Serial2 receive buffer
 * Prevents buffer overflow and ensures clean communication
 */
void clearSerialBuffer() {
  while (Serial2.available()) {
    Serial2.read();
  }
}

/**
 * Calculate CRC-16 for Modbus RTU protocol
 * Uses polynomial 0xA001 (reversed representation of 0x8005)
 * 
 * @param data Pointer to data array for CRC calculation
 * @param length Number of bytes to include in CRC calculation
 * @return 16-bit CRC value
 */
uint16_t calculateCRC(uint8_t *data, uint8_t length) {
  uint16_t crc = 0xFFFF; // Initialize CRC register
  
  // Process each byte in the data array
  for (uint8_t i = 0; i < length; i++) {
    crc ^= data[i]; // XOR byte with CRC register
    
    // Process each bit in the current byte
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        // If LSB is 1, shift right and XOR with polynomial
        crc = (crc >> 1) ^ 0xA001;
      } else {
        // If LSB is 0, just shift right
        crc >>= 1;
      }
    }
  }
  
  return crc;
}
